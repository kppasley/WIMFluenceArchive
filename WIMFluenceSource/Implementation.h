/*
WIMFluence calculates shear and moment influence lines and the resulting shear and moment reactions based on weigh-in-motion data. Copyright © 2016-2018 Kenneth Pasley

This file is part of WIMFluence.

WIMFluence is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

WIMFluence is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with WIMFluence.  If not, see http://www.gnu.org/licenses.
*/

#ifndef IMPLEMENTATION_H_INCLUDED
#define IMPLEMENTATION_H_INCLUDED

#include <iostream>     //input/output source file
#include <algorithm>
#include <math.h>
#include "Outputs.h"
//#include <Eigen/Dense>

#include "Structs.h"
#include "Prints.h"

using std::cout;
//using namespace Eigen;

extern bool write_all_response_file;

truck hs20_truck();

truck reverse_truck(truck truck_in);

void single_axle_forces(float span_length, float analysis_point, axle this_axle, float& point_shear, float& point_moment);

void truck_forces(float span_length, float analysis_point, truck& this_truck, float& truck_shear, float& truck_moment);

void iterate_truck(bridge& this_bridge, truck& forward_truck, map<float, extreme_set>& ap_extremes);

void two_span_moment_mid_support(float L1, float n, vector<float>& moment_IL_mid_support);

float left_span_moment(float x, float L1, float n);

float right_span_moment(float xi, float L1, float n);

float simple_span(bridge& this_bridge, float ordinate_location, float analysis_point_location);

float displacement(bridge& this_bridge, float ordinate_location, float analysis_point_location);

float displacement_ratio(bridge& this_bridge, float ordinate_location, float analysis_point_location);

void simple_set(bridge& this_bridge);

void delta_set(bridge& this_bridge);

vector< vector<float> > gaussian_elimination(vector< vector<float> > matrix);

void solve_factors(bridge& this_bridge);

void moment_supports(bridge& this_bridge, float ordinate_location);

void moment_ordinates(bridge& this_bridge, float ordinate_location, float analysis_point_location);

void shear_ordinates(bridge& this_bridge, float ordinate_location, float analysis_point_location);

#endif // IMPLEMENTATION_H_INCLUDED
