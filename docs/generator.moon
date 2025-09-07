#!/usr/bin/env moon

import insert, concat from table
import match, gsub, gmatch from string

config =
  source_dir: "../src/LuaEngine/Methods/"
  output_dir: "./generated/"
  method_files: {"PlayerMethods.hpp", "CreatureMethods.hpp", "GameObjectMethods.hpp", "ItemMethods.hpp", "GlobalMethods.hpp"}

ensure_directory = (path) -> os.execute "mkdir -p #{path}"
read_file = (filepath) ->
  file = io.open filepath, "r"
  return nil unless file
  content = file\read "*all"
  file\close!
  content

write_file = (filepath, content) ->
  file = io.open filepath, "w"
  return false unless file
  file\write content
  file\close!
  true

extract_class_name = (filename) -> filename\match "(.+)Methods%.hpp"

parse_documentation_block = (comment_block) ->
  doc = {brief: {}, enums: {}, examples: {}, params: {}, returns: "", notes: {}, warnings: {}, since: "", see_also: {}}
  lines = [line for line in comment_block\gmatch "[^\r\n]+"]
  
  in_code_block, in_enum_block, code_content, enum_content = false, false, {}, {}

  for line in *lines
    clean_line = line\gsub "^%s*%*%s?", ""
    
    if clean_line\match "@code"
      in_code_block, code_content = true, {}
    elseif clean_line\match "@endcode"
      in_code_block = false
      insert doc.examples, concat code_content, "\n"
      code_content = {}
    elseif in_code_block
      insert code_content, clean_line
    elseif clean_line\match "@brief"
      brief = clean_line\match "@brief%s+(.+)"
      insert doc.brief, brief if brief
    elseif clean_line\match "@param"
      param = clean_line\match "@param%s+(.+)"
      insert doc.params, param if param
    elseif clean_line\match "@return"
      doc.returns = clean_line\match("@return%s+(.+)") or ""
    elseif clean_line\match "@enum"
      unless in_enum_block
        in_enum_block, enum_content = true, {}
      enum_line = clean_line\match "@enum%s+(.+)"
      insert enum_content, enum_line if enum_line
    elseif clean_line\match "@note"
      if in_enum_block
        insert doc.enums, concat enum_content, "\n"
        in_enum_block, enum_content = false, {}
      note = clean_line\match "@note%s+(.+)"
      insert doc.notes, note if note
    elseif clean_line\match "@warning"
      warning = clean_line\match "@warning%s+(.+)"
      insert doc.warnings, warning if warning
    elseif clean_line\match "@since"
      doc.since = clean_line\match("@since%s+(.+)") or ""
    elseif clean_line\match "@see"
      see = clean_line\match "@see%s+(.+)"
      insert doc.see_also, see if see
    elseif in_enum_block and clean_line != ""
      insert enum_content, clean_line

  insert doc.enums, concat enum_content, "\n" if in_enum_block and #enum_content > 0
  doc

extract_method_signature = (cpp_line) ->
  return_type, method_name, params = cpp_line\match "inline%s+([%w_:%*&<>%s]+)%s+([%w_]+)%s*%((.-)%)"
  return nil unless return_type and method_name and params
  {
    return_type: return_type\gsub("%s+", " ")\gsub("^%s+", "")\gsub("%s+$", "")
    name: method_name
    parameters: params
  }

parse_method_file = (filepath) ->
  content = read_file filepath
  return {} unless content

  methods, current_comment = {}, nil
  lines = [line for line in content\gmatch "[^\r\n]+"]

  i = 1
  while i <= #lines
    line = lines[i]\gsub("^%s+", "")\gsub("%s+$", "")

    if line\match "^/%*%*"
      comment_lines = {line}
      i += 1
      while i <= #lines and not lines[i]\match "%*/"
        insert comment_lines, lines[i]
        i += 1
      insert comment_lines, lines[i] if i <= #lines
      current_comment = concat comment_lines, "\n"
    elseif line\match("^inline") and current_comment
      signature = extract_method_signature line
      if signature
        doc = parse_documentation_block current_comment
        insert methods, {name: signature.name, signature: signature, documentation: doc, raw_comment: current_comment}
      current_comment = nil

    i += 1
  methods

convert_class_references = (text, current_class) ->
  text\gsub "%[(%w+)%]", (class_ref) ->
    if class_ref == current_class then "[#{class_ref}](./index.md)" else "[#{class_ref}](../#{class_ref}/index.md)"

has_detailed_event_format = (enum_content) ->
  for line in *[line for line in enum_content\gmatch "[^\r\n]+"]
    return true if line and line\match("Context:") and line\match("Args:")
  false

parse_event_enum_line = (line) ->
  enum_name, id, comment = line\match "^%s*([%w_]+)%s*=%s*(%d+),?%s*//(.+)"
  return nil unless enum_name and id and comment

  description, context, args = comment\match "^%s*(.-)%s*|%s*Context:%s*(.-)%s*|%s*Args:%s*(.+)"
  return nil unless description and context and args

  {
    id: id, name: enum_name
    description: description\gsub("^%s+", "")\gsub("%s+$", "")
    context: context\gsub("^%s+", "")\gsub("%s+$", "")
    args: args\gsub("^%s*%(", "")\gsub("%)%s*$", "")\gsub("^%s+", "")\gsub("%s+$", "")
  }

generate_method_markdown = (method, class_name) ->
  md = {}

  insert md, "# #{method.name}"
  insert md, ""

  if #method.documentation.brief > 0
    for brief in *method.documentation.brief
      linked_brief = convert_class_references brief, class_name
      insert md, linked_brief
    insert md, ""

  insert md, "## Signature"
  insert md, ""
  insert md, "```cpp"
  insert md, "#{method.signature.return_type} #{method.name}(#{method.signature.parameters})"
  insert md, "```"
  insert md, ""

  if #method.documentation.params > 0
    insert md, "## Parameters"
    insert md, ""
    for param in *method.documentation.params
      if param\match "%[%w+%]"
        before, class_ref, after = param\match "(.-)%[(%w+)%](.*)"
        if class_ref and after
          link = if class_ref == class_name then "[#{class_ref}](./index.md)" else "[#{class_ref}](../#{class_ref}/index.md)"
          insert md, "- #{before}#{link}`#{after}`"
        else
          insert md, "- `#{param}`"
      else
        insert md, "- `#{param}`"
    insert md, ""

  if method.documentation.returns != ""
    insert md, "## Returns"
    insert md, ""
    insert md, method.documentation.returns
    insert md, ""

  if #method.documentation.enums > 0
    insert md, "## Enums"
    insert md, ""

    for enum_content in *method.documentation.enums
      if enum_content\match "^%s*|.*|\n|%s*|-.*-|"
        insert md, enum_content
      elseif has_detailed_event_format enum_content
        insert md, "| Event ID | Event Name | Arguments | Context | Description |"
        insert md, "|----------|------------|-----------|---------|-------------|"

        for line in *[line for line in enum_content\gmatch "[^\r\n]+"]
          continue unless line and line != ""
          continue if line\match("^%s*[{}]%s*$") or line\match("^%s*/$")

          event = parse_event_enum_line line
          insert md, "| #{event.id} | #{event.name} | (#{event.args}) | #{event.context} | #{event.description} |" if event
      else
        insert md, "```cpp"
        for line in *[line for line in enum_content\gmatch "[^\r\n]+"]
          continue if line\match "^%s*/$"
          insert md, line if line and line != ""
        insert md, "```"

    insert md, ""

  if #method.documentation.examples > 0
    insert md, "## Examples"
    insert md, ""
    for i, example in ipairs method.documentation.examples
      insert md, "```lua"
      insert md, example
      insert md, "```"
      insert md, "" if i < #method.documentation.examples
    insert md, ""

  if #method.documentation.notes > 0
    insert md, "## Notes"
    insert md, ""
    insert md, "> **Note:** #{note}" for note in *method.documentation.notes
    insert md, ""

  if #method.documentation.warnings > 0
    insert md, "## Warnings"
    insert md, ""
    insert md, "> **⚠️ Warning:** #{warning}" for warning in *method.documentation.warnings
    insert md, ""

  if #method.documentation.see_also > 0
    insert md, "## See Also"
    insert md, ""
    for see in *method.documentation.see_also
      class_name_ref = see\match "([%w_]+)::"
      if class_name_ref
        method_name_ref = see\match "::([%w_]+)"
        link = if method_name_ref
          "- [#{see}](../#{class_name_ref}/#{method_name_ref\lower!}.md)"
        else
          "- [#{see}](../#{class_name_ref}/index.md)"
        insert md, link
      else
        insert md, "- #{see}"
    insert md, ""

  concat md, "\n"

generate_class_index = (class_name, methods) ->
  md = {
    "# #{class_name} Methods"
    ""
    "This page contains all available methods for the `#{class_name}` class in Eclipse Lua scripting."
    ""
    "## Available Methods"
    ""
    "| Method | Description |"
    "|--------|-------------|"
  }

  for method in *methods
    brief = if #method.documentation.brief > 0
      convert_class_references method.documentation.brief[1]\gsub("^%s+", "")\gsub("%s+$", ""), class_name
    else
      ""
    insert md, "| [#{method.name}](./#{method.name\lower!}.md) | #{brief} |"

  concat md, "\n"

generate = ->
  print "Eclipse Documentation Generator (MoonScript)"
  print "==========================================="

  ensure_directory config.output_dir
  all_classes = {}

  for method_file in *config.method_files
    filepath = config.source_dir .. method_file
    print "Processing: #{method_file}"

    class_name = extract_class_name method_file
    if class_name
      methods = parse_method_file filepath

      if #methods > 0
        all_classes[class_name] = methods
        class_dir = config.output_dir .. class_name .. "/"
        ensure_directory class_dir

        for method in *methods
          method_md = generate_method_markdown method, class_name
          write_file class_dir .. method.name\lower! .. ".md", method_md

        write_file class_dir .. "index.md", generate_class_index class_name, methods
        print "  Generated #{#methods} method docs for #{class_name}"
      else
        print "  No documented methods found in #{method_file}"
    else
      print "  Could not extract class name from #{method_file}"

  main_index = {
    "# Eclipse Lua API Documentation"
    ""
    "Welcome to the Eclipse Lua scripting API documentation."
    ""
    "## Available Classes"
    ""
  }

  for class_name, methods in pairs all_classes
    insert main_index, "- [#{class_name}](./#{class_name}/index.md) (#{#methods} methods)"

  insert main_index, ""
  insert main_index, "---"
  insert main_index, "*Generated automatically by Eclipse Documentation Generator*"

  write_file config.output_dir .. "index.md", concat main_index, "\n"

  print "\nDocumentation generation complete!"
  print "Output directory: #{config.output_dir}"

generate!