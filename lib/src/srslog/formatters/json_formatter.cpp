/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2021 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#include "json_formatter.h"
#include "srsran/srslog/detail/log_entry_metadata.h"

using namespace srslog;

std::unique_ptr<log_formatter> json_formatter::clone() const
{
  return std::unique_ptr<log_formatter>(new json_formatter);
}

void json_formatter::format(detail::log_entry_metadata&& metadata, fmt::memory_buffer& buffer)
{
  fmt::format_to(buffer,
                 "{{\n"
                 "  \"log_entry\": \"");
  if (metadata.fmtstring) {
    if (metadata.store) {
      fmt::basic_format_args<fmt::basic_printf_context_t<char> > args(*metadata.store);
      fmt::vprintf(buffer, fmt::to_string_view(metadata.fmtstring), args);
      fmt::format_to(buffer, fmt::to_string_view("\""));
    } else {
      fmt::format_to(buffer, "{}\"", metadata.fmtstring);
    }
  }

  if (!metadata.hex_dump.empty()) {
    fmt::format_to(
        buffer, ",\n  \"hex_dump\": \"{:02x}\"", fmt::join(metadata.hex_dump.cbegin(), metadata.hex_dump.cend(), " "));
  }

  fmt::format_to(buffer, "\n}}\n");
}

void json_formatter::format_context_begin(const detail::log_entry_metadata& md,
                                          fmt::string_view                  ctx_name,
                                          unsigned                          size,
                                          fmt::memory_buffer&               buffer)
{
  assert(scope_stack.empty() && "Stack should be empty");
  assert(nest_level == 0 && "Nesting level should be 0");

  fmt::format_to(buffer, "{{\n");
  push_scope(size);

  if (md.fmtstring) {
    if (md.store) {
      fmt::format_to(buffer, "  \"log_entry\": \"");
      fmt::basic_format_args<fmt::basic_printf_context_t<char> > args(*md.store);
      fmt::vprintf(buffer, fmt::to_string_view(md.fmtstring), args);
      fmt::format_to(buffer, "\",\n");
    } else {
      fmt::format_to(buffer, "  \"log_entry\": \"{}\",\n", md.fmtstring);
    }
  }
}

void json_formatter::format_context_end(const detail::log_entry_metadata& md,
                                        fmt::string_view                  ctx_name,
                                        fmt::memory_buffer&               buffer)
{
  pop_scope();
  fmt::format_to(buffer, "}}\n");

  assert(scope_stack.empty() && "Stack should be empty");
  assert(nest_level == 0 && "Nesting level should be 0");
}

void json_formatter::format_metric_set_begin(fmt::string_view    set_name,
                                             unsigned            size,
                                             unsigned            level,
                                             fmt::memory_buffer& buffer)
{
  // Arrays in JSON require an additional nesting level before inserting the
  // object.
  // array: [
  //   {
  //     "obj: {}
  //   }
  // ]
  if (in_list_scope()) {
    fmt::format_to(buffer, "{: <{}}{{\n", ' ', indents(level));
    increment_nest_level();
  }

  consume_element();

  fmt::format_to(buffer, "{: <{}}\"{}\": {{\n", ' ', indents(level), set_name);
  push_scope(size);
}

void json_formatter::format_metric_set_end(fmt::string_view set_name, unsigned level, fmt::memory_buffer& buffer)
{
  pop_scope();
  fmt::format_to(buffer, "{: <{}}}}{}\n", ' ', indents(level), needs_comma() && !in_list_scope() ? "," : "");

  if (in_list_scope()) {
    decrement_nest_level();
    fmt::format_to(buffer, "{: <{}}}}{}\n", ' ', indents(level), needs_comma() ? "," : "");
  }
}

void json_formatter::format_metric(fmt::string_view    metric_name,
                                   fmt::string_view    metric_value,
                                   fmt::string_view    metric_units,
                                   metric_kind         kind,
                                   unsigned            level,
                                   fmt::memory_buffer& buffer)
{
  consume_element();

  fmt::format_to(buffer,
                 "{: <{}}\"{}\": {}{}{}{}\n",
                 ' ',
                 indents(level),
                 metric_name,
                 kind == metric_kind::string ? "\"" : "",
                 metric_value,
                 kind == metric_kind::string ? "\"" : "",
                 needs_comma() ? "," : "");
}

void json_formatter::format_list_begin(fmt::string_view    list_name,
                                       unsigned            size,
                                       unsigned            level,
                                       fmt::memory_buffer& buffer)
{
  consume_element();

  fmt::format_to(buffer, "{: <{}}\"{}\": [\n", ' ', indents(level), list_name);
  push_list_scope(size);
}

void json_formatter::format_list_end(fmt::string_view list_name, unsigned level, fmt::memory_buffer& buffer)
{
  pop_scope();
  fmt::format_to(buffer, "{: <{}}]{}\n", ' ', indents(level), needs_comma() ? "," : "");
}