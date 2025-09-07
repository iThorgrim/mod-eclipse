#!/usr/bin/env moon

-- Eclipse Documentation Generator (Moonscript Version)
--
-- This script parses C++ method files from mod-eclipse and generates
-- comprehensive Markdown documentation organized by class types.
--
-- Usage: moon generator.moon

import insert from table
import concat from table
import match, gsub, gmatch from string

-- Configuration
config =
  source_dir: "../src/LuaEngine/Methods/"
  output_dir: "./generated/"
  method_files: {
    "PlayerMethods.hpp"
    "CreatureMethods.hpp"
    "GameObjectMethods.hpp"
    "ItemMethods.hpp"
    "GlobalMethods.hpp"
  }

-- Utility functions
ensure_directory = (path) ->
  os.execute "mkdir -p #{path}"

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

extract_class_name = (filename) ->
  -- PlayerMethods.hpp -> Player
  filename\match "(.+)Methods%.hpp"

-- Parser for C++ documentation comments
parse_documentation_block = (comment_block) ->
  doc =
    brief: {}
    description: ""
    enums: {}
    examples: {}
    params: {}
    returns: ""
    notes: {}
    warnings: {}
    since: ""
    see_also: {}

  lines = [line for line in comment_block\gmatch "[^\r\n]+"]

  in_code_block = false
  code_content = {}

  for line in *lines
    line = line\gsub "^%s*%*%s?", ""

    if line\match "@code"
      in_code_block = true
      code_content = {}
    elseif line\match "@endcode"
      in_code_block = false
      insert doc.examples, concat code_content, "\n"
    elseif in_code_block
      insert code_content, line
    elseif line\match "@brief"
      brief = line\match "@brief%s+(.+)"
      insert doc.brief, brief if brief
    elseif line\match "@param"
      param = line\match "@param%s+(.+)"
      insert doc.params, param if param
    elseif line\match "@return"
      doc.returns = line\match("@return%s+(.+)") or ""
    elseif line\match "@enum"
      enum_line = line\match "@enum%s+(.+)"
      insert doc.enums, enum_line if enum_line
    elseif line\match "@note"
      note = line\match "@note%s+(.+)"
      insert doc.notes, note if note
    elseif line\match "@warning"
      warning = line\match "@warning%s+(.+)"
      insert doc.warnings, warning if warning
    elseif line\match "@since"
      doc.since = line\match("@since%s+(.+)") or ""
    elseif line\match "@see"
      see = line\match "@see%s+(.+)"
      insert doc.see_also, see if see

  doc

-- Extract method signature from C++ code
extract_method_signature = (cpp_line) ->
  return_type, method_name, params = cpp_line\match "inline%s+([%w_:%*&<>%s]+)%s+([%w_]+)%s*%((.-)%)"

  if return_type and method_name and params
    {
      return_type: return_type\gsub("%s+", " ")\gsub("^%s+", "")\gsub("%s+$", "")
      name: method_name
      parameters: params
    }
  else
    nil

-- Parse a single method file
parse_method_file = (filepath) ->
  content = read_file filepath
  return {} unless content

  methods = {}
  current_comment = nil
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

      if i <= #lines
        insert comment_lines, lines[i]

      current_comment = concat comment_lines, "\n"

    elseif line\match("^inline") and current_comment
      signature = extract_method_signature line
      if signature
        doc = parse_documentation_block current_comment

        insert methods, {
          name: signature.name
          signature: signature
          documentation: doc
          raw_comment: current_comment
        }
      current_comment = nil

    i += 1

  methods

-- Convert class references to links
convert_class_references = (text, current_class) ->
  text\gsub "%[(%w+)%]", (class_ref) ->
    if class_ref == current_class
      "[#{class_ref}](./index.md)"
    else
      "[#{class_ref}](../#{class_ref}/index.md)"

-- Generate markdown for a single method
generate_method_markdown = (method, class_name) ->
  md = {}

  -- Header
  insert md, "# #{method.name}"
  insert md, ""

  -- Brief description
  if #method.documentation.brief > 0
    for brief in *method.documentation.brief
      linked_brief = convert_class_references brief, class_name
      insert md, linked_brief
    insert md, ""

  -- Method signature
  insert md, "## Signature"
  insert md, ""
  insert md, "```cpp"
  insert md, "#{method.signature.return_type} #{method.name}(#{method.signature.parameters})"
  insert md, "```"
  insert md, ""

  -- Parameters
  if #method.documentation.params > 0
    insert md, "## Parameters"
    insert md, ""
    for param in *method.documentation.params
      insert md, "- `#{param}`"
    insert md, ""

  -- Return value
  if method.documentation.returns != ""
    insert md, "## Returns"
    insert md, ""
    insert md, method.documentation.returns
    insert md, ""

  -- Enums
  if #method.documentation.enums > 0
    insert md, "## Enums"
    insert md, ""
    insert md, "```cpp"
    for enum_line in *method.documentation.enums
      insert md, enum_line
    insert md, "```"
    insert md, ""

  -- Examples
  if #method.documentation.examples > 0
    insert md, "## Examples"
    insert md, ""
    for i, example in ipairs method.documentation.examples
      insert md, "```lua"
      insert md, example
      insert md, "```"
      insert md, "" if i < #method.documentation.examples
    insert md, ""

  -- Notes
  if #method.documentation.notes > 0
    insert md, "## Notes"
    insert md, ""
    for note in *method.documentation.notes
      insert md, "> **Note:** #{note}"
    insert md, ""

  -- Warnings
  if #method.documentation.warnings > 0
    insert md, "## Warnings"
    insert md, ""
    for warning in *method.documentation.warnings
      insert md, "> **⚠️ Warning:** #{warning}"
    insert md, ""

  -- See also
  if #method.documentation.see_also > 0
    insert md, "## See Also"
    insert md, ""
    for see in *method.documentation.see_also
      -- Convert class references to links
      class_name_ref = see\match "([%w_]+)::"
      if class_name_ref
        method_name_ref = see\match "::([%w_]+)"
        if method_name_ref
          insert md, "- [#{see}](../#{class_name_ref}/#{method_name_ref\lower!}.md)"
        else
          insert md, "- [#{see}](../#{class_name_ref}/index.md)"
      else
        insert md, "- #{see}"
    insert md, ""

  concat md, "\n"

-- Generate index file for a class
generate_class_index = (class_name, methods) ->
  md = {}

  insert md, "# #{class_name} Methods"
  insert md, ""
  insert md, "This page contains all available methods for the `#{class_name}` class in Eclipse Lua scripting."
  insert md, ""

  -- Table of contents
  insert md, "## Available Methods"
  insert md, ""
  insert md, "| Method | Description |"
  insert md, "|--------|-------------|"

  for method in *methods
    brief = ""
    if #method.documentation.brief > 0
      brief = method.documentation.brief[1]\gsub("^%s+", "")\gsub("%s+$", "")
      brief = convert_class_references brief, class_name

    insert md, "| [#{method.name}](./#{method.name\lower!}.md) | #{brief} |"

  concat md, "\n"

generate = ->
  print "Eclipse Documentation Generator (Moonscript)"
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
          method_file_path = class_dir .. method.name\lower! .. ".md"
          write_file method_file_path, method_md

        index_md = generate_class_index class_name, methods
        write_file class_dir .. "index.md", index_md

        print "  Generated #{#methods} method docs for #{class_name}"
      else
        print "  No documented methods found in #{method_file}"
    else
      print "  Could not extract class name from #{method_file}"

  main_index = {}
  insert main_index, "# Eclipse Lua API Documentation"
  insert main_index, ""
  insert main_index, "Welcome to the Eclipse Lua scripting API documentation."
  insert main_index, ""
  insert main_index, "## Available Classes"
  insert main_index, ""

  for class_name, methods in pairs all_classes
    insert main_index, "- [#{class_name}](./#{class_name}/index.md) (#{#methods} methods)"

  insert main_index, ""
  insert main_index, "---"
  insert main_index, "*Generated automatically by Eclipse Documentation Generator*"

  write_file config.output_dir .. "index.md", concat main_index, "\n"

  print "\nDocumentation generation complete!"
  print "Output directory: #{config.output_dir}"

-- Run the generator
generate!