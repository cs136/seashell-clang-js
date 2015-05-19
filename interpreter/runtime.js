/**
 * Seashell C Runtime Library (JavaScript bindings)
 * Copyright (C) 2013-2015 The Seashell Maintainers.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * See also 'ADDITIONAL TERMS' at the end of the included LICENSE file.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with self program.  If not, see <http://www.gnu.org/licenses/>.
 */
Module._RT_extcall_result = 0;
Module._RT_stdin_buffer = "";
Module._RT_resume_next = "";
Module._RT_error = "";
/**
 * _RT_internal_error(std::string error)
 * Handles internal errors.  Needs to be implemented
 * by whatever is using the interpreter.
 */
Module._RT_internal_error = function(error) {
  Module._RT_error = "";
};

/**
 * _RT_suspend_function(std::string name)
 * Registers the function that needs to be resumed later.
 */
Module._RT_suspend_function = function(name) {
  Module._RT_resume_next = name;
};

/**
 * int _RT_suspend(void)
 * 
 * Returns: 5. (after resuming from SSS _suspend)
 * (Used to test suspend/resume in the interpreter.
 */
Module._RT_suspend = function() {
  "use strict";

  Module._RT_extcall_result = 5;
};

/**
 * std::string _RT_stdin_read(int wanted)
 *
 * Returns: Up to wanted bytes from standard input,
 *          or an integer:
 *            0 - denoting end of file.
 *            -1 - denoting empty buffer (can be resumed later).
 *
 *          This call can be resumed after the exception
 *          after the buffer Module._RT_stdin_buffer has been filled.
 */
Module._RT_stdin_read = function(wanted) {
  "use strict";
  if (Module._RT_stdin_buffer.length === 0) {
    return -1;
  }

  var result = Module._RT_stdin_buffer.substr(0, wanted);
  Module._RT_stdin_buffer = Module._RT_stdin_buffer.substr(wanted);
  
  return result;
};

/**
 * void _RT_stdout_write(std::string str)
 *
 * Writes str out to stdout.  This function
 * is meant to be overridden by whatever is using
 * this library.
 *
 * By default, just runs console.log
 */
Module._RT_stdout_write = console.log;

/**
 * (void) _RT_stdin_fill(std::string buffer)
 *
 * Fills the standard input buffer.
 * Returns: Nothing.
 */
Module._RT_stdin_fill = function(buffer) {
  "use strict";

  Module._RT_stdin_buffer += buffer;
};
