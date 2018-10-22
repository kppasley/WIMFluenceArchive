/*
WIMFluence calculates shear and moment influence lines and the resulting shear and moment reactions based on weigh-in-motion data. Copyright © 2016-2018 Kenneth Pasley

This file is part of WIMFluence.

WIMFluence is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

WIMFluence is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with WIMFluence.  If not, see http://www.gnu.org/licenses.
*/

#ifndef INPUTS_H_INCLUDED
#define INPUTS_H_INCLUDED

#include <fstream>
#include <iostream>     //input/output source file
#include <string>
#include <sstream>
#include <iomanip>
#include <math.h>
#include "Structs.h"
#include <windows.h>
#include <sys/stat.h>

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::stringstream;
//using std::to_string;
//using std::round;

bool file_exists(const std::string& file);

void open_input_wim_file(ifstream& wim_in, string& wim_file_name);

void open_output_shear_file(ofstream& shear_out, string wim_file_name, bridge& this_bridge);

void open_output_moment_file(ofstream& moment_out, string wim_file_name, bridge& this_bridge);

void open_output_influence_line_file(ofstream& influence_line_out, bridge& this_bridge);

void open_output_analysis_point_extremes_file(ofstream& analysis_point_extremes_out, string wim_file_name, bridge& this_bridge);

void open_output_truck_file(ofstream& truck_out, string wim_file_name, bridge& this_bridge);

void open_output_all_response_file(ofstream& all_response_out, string wim_file_name, bridge& this_bridge);

void open_output_extreme_response_file(ofstream& extreme_response_out, string wim_file_name, bridge& this_bridge);

void get_bridge(bridge& new_bridge);

truck get_truck(string line, string units_bool);


#endif // INPUTS_H_INCLUDED
