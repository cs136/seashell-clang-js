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

/**
 * int _RT_suspend(void)
 * 
 * Returns: 5.
 * (Used to test suspend/resume in the interpreter.
 */
Module._RT_suspend = function() {
  Module._RT_extcall_result = 5;
  throw "SSS _suspend";
};

