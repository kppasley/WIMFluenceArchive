/*
WIMFluence calculates shear and moment influence lines and the resulting shear and moment reactions based on weigh-in-motion data. Copyright © 2016-2018 Kenneth Pasley

This file is part of WIMFluence.

WIMFluence is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

WIMFluence is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with WIMFluence.  If not, see http://www.gnu.org/licenses.
*/

#ifndef OUTPUTS_H_INCLUDED
#define OUTPUTS_H_INCLUDED

#include <vector>
#include <fstream>
#include <iostream>     //input/output source file
#include "Structs.h"
#include "iomanip"

using std::vector;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::fixed;
using std::setprecision;
using std::setw;
using std::cout;

extern ofstream all_response_out;

void write_bridge_info(bridge& this_bridge, ofstream& file_out);

//void write_shear(extreme_set& extremes, ofstream& shear_out);

//void write_shear2(extreme_set& extremes, ofstream& shear_out);

//void write_moment(extreme_set& extremes, ofstream& moment_out);

//void write_moment2(extreme_set& extremes, ofstream& moment_out);

void write_influence_line1(bridge& this_bridge, ofstream& influence_line_out);

void write_influence_line2(bridge& this_bridge, ofstream& influence_line_out);

//void write_analysis_point_extremes(ofstream&, map<float, extreme_set>& ap_extremes, bridge& this_bridge);

void write_truck(ofstream& truck_out, truck& this_truck);

void write_all_response(truck& this_truck, float analysis_point, float moment, float shear_left, float shear_right);

void write_extreme_response(ofstream& extreme_response_out, map<float, extreme_set>& ap_extremes);


#endif // OUTPUTS_H_INCLUDED
