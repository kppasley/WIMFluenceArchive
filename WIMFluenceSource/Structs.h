/*
WIMFluence calculates shear and moment influence lines and the resulting shear and moment reactions based on weigh-in-motion data. Copyright © 2016-2018 Kenneth Pasley

This file is part of WIMFluence.

WIMFluence is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

WIMFluence is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with WIMFluence.  If not, see http://www.gnu.org/licenses.
*/

#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

#include <vector>
#include <map>
#include <limits>
#include <string>

using std::vector;
using std::map;
using std::numeric_limits;
using std::string;


//---------------------------
//Structures
//---------------------------

struct axle
{
    float position;
    float weight;
};

struct truck
{
    int number = 0;
    int num_axles = 0;
    vector<axle> axles;
    char direction = 'f';
};

struct extreme_set
{
    truck max_shear_left_truck;
    truck min_shear_left_truck;
    truck max_shear_right_truck;
    truck min_shear_right_truck;
    truck max_moment_truck;
    truck min_moment_truck;
    float max_shear_left = -numeric_limits<float>::max();//sets the initial MAX value to the MINIMUM float value to ensure the first SHEAR replaces it
    float min_shear_left = numeric_limits<float>::max();//sets the initial MIN value to the MAXIMUM float value to ensure the first SHEAR replaces it
    float max_shear_right = -numeric_limits<float>::max();//sets the initial MAX value to the MINIMUM float value to ensure the first SHEAR replaces it
    float min_shear_right = numeric_limits<float>::max();//sets the initial MIN value to the MAXIMUM float value to ensure the first SHEAR replaces it
    float max_moment = -numeric_limits<float>::max();//sets the initial MAX value to the MINIMUM float value to ensure the first MOMENT replaces it
    float min_moment = numeric_limits<float>::max();//sets the initial MAX value to the MAXIMUM float value to ensure the first MOMENT replaces it
    //float max_shear_analysis_point = 0;
    //float min_shear_analysis_point = 0;
    //float max_moment_analysis_point = 0;
    //float min_moment_analysis_point = 0;
};

/*struct analysis_point_extreme_set
{
    truck this_truck;

    float max_shear = -numeric_limits<float>::max();//sets the initial MAX value to the MINIMUM float value to ensure the first SHEAR replaces it

    float min_shear = numeric_limits<float>::max();//sets the initial MIN value to the MAXIMUM float value to ensure the first SHEAR replaces it

    float max_moment = -numeric_limits<float>::max();//sets the initial MAX value to the MINIMUM float value to ensure the first MOMENT replaces it

    float min_moment = numeric_limits<float>::max();//sets the initial MAX value to the MAXIMUM float value to ensure the first MOMENT replaces it
};*/

struct bridge
{
    int spans = 0;
    float length_span_one = 0;
    float length_total = 0;
    vector<float> span_lengths;
    vector<float> support_positions;
    vector< vector<float> > support_simple_set;
    vector< vector<float> > support_delta_set;
    vector< vector<float> > support_factors;
    map<float, vector<float> > support_ordinates; //using a map allows the storage of ordinates for any location
    map<float, map<float, float> > shear_ordinates_left; //analysis point, load point, ordinate; shear from the left
    map<float, map<float, float> > shear_ordinates_right; //analysis point, load point, ordinate; shear from the right
    map<float, map<float, float> > moment_ordinates; //analysis point, load point, ordinate; moment
};


#endif // STRUCTS_H_INCLUDED
