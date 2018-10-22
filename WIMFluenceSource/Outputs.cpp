/*
WIMFluence calculates shear and moment influence lines and the resulting shear and moment reactions based on weigh-in-motion data. Copyright © 2016-2018 Kenneth Pasley

This file is part of WIMFluence.

WIMFluence is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

WIMFluence is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with WIMFluence.  If not, see http://www.gnu.org/licenses.
*/

#include "Outputs.h"

void write_bridge_info(bridge& this_bridge, ofstream& file_out)
{
    file_out << "spans:," << this_bridge.spans;

    file_out << "\nspan lengths:";

    for(int i = 0; i < this_bridge.spans; i++){
        file_out << "," << this_bridge.span_lengths[i];
    }

    file_out << "\nspan length ratios:";

    for(int i = 0; i < this_bridge.spans; i++){
        file_out << "," << this_bridge.span_lengths[i] / this_bridge.span_lengths[0];
    }

    file_out << "\ninternal support positions:";
    float position = 0;
    for(int i = 0; i < this_bridge.spans - 1; i++){
        position += this_bridge.span_lengths[i];
        file_out << "," << position;
    }

    file_out << "\ninternal support position ratios:";
    position = 0;
    for(int i = 0; i < this_bridge.spans - 1; i++){
        position += this_bridge.span_lengths[i];
        float position_ratio = position / this_bridge.span_lengths[0];
        file_out << "," << position_ratio;
    }
}

/*void write_shear(extreme_set& extremes, ofstream& shear_out)
{
    shear_out << "max shear: \t\t\t" << extremes.max_shear << endl
        << "analysis point: \t" << extremes.max_shear_analysis_point << endl
        << "max shear truck: " << endl
        << "number of axles: \t" << extremes.max_shear_truck.num_axles << endl;
    for(int i = 0; i < extremes.max_shear_truck.num_axles; i++){
        shear_out << "axle " << i+1 << " weight: \t\t" << extremes.max_shear_truck.axles[i].weight << endl;
        shear_out << "axle " << i+1 << " position: \t" << extremes.max_shear_truck.axles[i].position << endl;
    }

    shear_out << "------------------------------" << endl;

    shear_out << "min shear: \t\t\t" << extremes.min_shear << endl
        << "analysis point: \t" << extremes.min_shear_analysis_point << endl
        << "min shear truck: " << endl
        << "number of axles: \t" << extremes.min_shear_truck.num_axles << endl;
    for(int i = 0; i < extremes.min_shear_truck.num_axles; i++){
        shear_out << "axle " << i+1 << " weight: \t\t" << extremes.min_shear_truck.axles[i].weight << endl;
        shear_out << "axle " << i+1 << " position: \t" << extremes.min_shear_truck.axles[i].position << endl;
    }

    shear_out << "==============================\n==============================" << endl;
}*/

/*void write_shear2(extreme_set& extremes, ofstream& shear_out)
{
    shear_out << extremes.max_shear << "," << extremes.max_shear_analysis_point;
    for(int i = 0; i < extremes.max_shear_truck.num_axles; i++){
        shear_out << "," << extremes.max_shear_truck.axles[i].weight << "," << extremes.max_shear_truck.axles[i].position;
    }

    shear_out << ",,," << extremes.min_shear << "," << extremes.min_shear_analysis_point;
    for(int i = 0; i < extremes.max_shear_truck.num_axles; i++){
        shear_out << "," << extremes.min_shear_truck.axles[i].weight << "," << extremes.min_shear_truck.axles[i].position;
    }

    shear_out << endl;
}*/

/*void write_moment(extreme_set& extremes, ofstream& moment_out)
{
    moment_out << "max moment: \t\t" << extremes.max_moment << endl
        << "analysis point: \t" << extremes.max_moment_analysis_point << endl
        << "max moment truck: " << endl
        << "number of axles: \t" << extremes.max_moment_truck.num_axles << endl;
    for(int i = 0; i < extremes.max_moment_truck.num_axles; i++){
        moment_out << "axle " << i+1 << " weight: \t\t" << extremes.max_moment_truck.axles[i].weight << endl;
        moment_out << "axle " << i+1 << " position: \t" << extremes.max_moment_truck.axles[i].position << endl;
    }

    moment_out << "-------------------------" << endl;

    moment_out << "min moment: \t\t" << extremes.min_moment << endl
        << "analysis point: \t" << extremes.min_moment_analysis_point << endl
        << "min moment truck: " << endl
        << "number of axles: \t" << extremes.min_moment_truck.num_axles << endl;
    for(int i = 0; i < extremes.min_moment_truck.num_axles; i++){
        moment_out << "axle " << i+1 << " weight: \t\t" << extremes.min_moment_truck.axles[i].weight << endl;
        moment_out << "axle " << i+1 << " position: \t" << extremes.min_moment_truck.axles[i].position << endl;
    }

    moment_out << "=========================\n=========================" << endl;
}*/

/*void write_moment2(extreme_set& extremes, ofstream& moment_out)
{
    moment_out << extremes.max_moment << "," << extremes.max_moment_analysis_point;
    for(int i = 0; i < extremes.max_moment_truck.num_axles; i++){
        moment_out << "," << extremes.max_moment_truck.axles[i].weight << "," << extremes.max_moment_truck.axles[i].position;
    }

    moment_out << ",,," << extremes.min_moment << "," << extremes.min_moment_analysis_point;
    for(int i = 0; i < extremes.max_moment_truck.num_axles; i++){
        moment_out << "," << extremes.min_moment_truck.axles[i].weight << "," << extremes.min_moment_truck.axles[i].position;
    }

    moment_out << endl;
}*/

void write_influence_line1(bridge& this_bridge, ofstream &influence_line_out)
{
    write_bridge_info(this_bridge, influence_line_out);

    influence_line_out << fixed;

    influence_line_out << "\n\nanalysis_point,analysis_point_ratio,load_point,moment,left_shear,right_shear";

    //'it_left' is the iterator of this_bridge.shear_ordinates_left
    //auto it_right = this_bridge.shear_ordinates_right.begin();

    for(auto it_left = this_bridge.shear_ordinates_left.begin(); it_left != this_bridge.shear_ordinates_left.end(); it_left++){
        //'it_left' is the iterator of this_bridge.shear_ordinates_left
        //'it_left' is the iterator of this_bridge.shear_ordinates_left
        //'first' is the float index of the map
        //'second' is the content of the map at index 'first'. in this case 'second' is the map of ordinate values for analysis point 'first'

        //output the location of the analysis point
        float analysis_point = it_left->first;

        for(auto it_left_ordinate = this_bridge.shear_ordinates_left[analysis_point].begin(); it_left_ordinate != this_bridge.shear_ordinates_left[analysis_point].end(); it_left_ordinate++){
            float ordinate_location = it_left_ordinate->first;

            influence_line_out << setprecision(4) << "\n" << analysis_point << "," << analysis_point / this_bridge.length_span_one << "," << ordinate_location << "," << this_bridge.moment_ordinates[analysis_point][ordinate_location] << "," << this_bridge.shear_ordinates_left[analysis_point][ordinate_location] << "," << this_bridge.shear_ordinates_right[analysis_point][ordinate_location];
        }
    }
}

void write_influence_line2(bridge& this_bridge, ofstream &influence_line_out)
{
    write_bridge_info(this_bridge, influence_line_out);

    influence_line_out << fixed;

    influence_line_out << "\n\nMOMENT\n,load point ratio";

    for(float i = 0; i <= this_bridge.length_total; i += 1.0){
        influence_line_out <<  "," << i / this_bridge.length_span_one;
    }

    influence_line_out << "\nanalysis point ratio,actual";

    for(float i = 0; i <= this_bridge.length_total; i += 1.0){
        influence_line_out << setprecision(1) << "," << i;
    }

    for(auto it = this_bridge.moment_ordinates.begin(); it != this_bridge.moment_ordinates.end(); it++){
        //'it' is the iterator of this_bridge.moment_ordinates
        //'first' is the float index
        //'second' is the content of the map at index 'first'. in this case 'second' is the map of ordinate values for analysis point 'first'

        //output the location of the analysis point
        influence_line_out << setprecision(10) << "\n" << (it->first) / this_bridge.length_span_one << setprecision(1) << "," << it->first;

        for(auto ordinate : (it->second)){
            influence_line_out << setprecision(10) << "," << ordinate.second;
        }
    }

    influence_line_out << "\n\n\n";

    influence_line_out << "\n\nSHEAR left\n,load point ratio";

    for(float i = 0; i <= this_bridge.length_total; i += 1.0){
        influence_line_out <<  "," << i / this_bridge.length_span_one;
    }

    influence_line_out << "\nanalysis point ratio,actual";

    for(float i = 0; i <= this_bridge.length_total; i += 1.0){
        influence_line_out << setprecision(1) << "," << i;
    }

    for(auto it = this_bridge.shear_ordinates_left.begin(); it != this_bridge.shear_ordinates_left.end(); it++){
        //'it' is the iterator of this_bridge.moment_ordinates
        //'first' is the float index
        //'second' is the content of the map at index 'first'. in this case 'second' is the map of ordinate values for analysis point 'first'

        //output the location of the analysis point
        influence_line_out << setprecision(4) << "\n" << (it->first) / this_bridge.length_span_one << setprecision(1) << "," << it->first;

        for(auto ordinate : (it->second)){
            influence_line_out << setprecision(4) << "," << ordinate.second;
        }
    }

    influence_line_out << "\n\n\n";

    influence_line_out << "\n\nSHEAR right\n,load point ratio";

    for(float i = 0; i <= this_bridge.length_total; i += 1.0){
        influence_line_out <<  "," << i / this_bridge.length_span_one;
    }

    influence_line_out << "\nanalysis point ratio,actual";

    for(float i = 0; i <= this_bridge.length_total; i += 1.0){
        influence_line_out << setprecision(1) << "," << i;
    }

    for(auto it = this_bridge.shear_ordinates_right.begin(); it != this_bridge.shear_ordinates_right.end(); it++){
        //'it' is the iterator of this_bridge.moment_ordinates
        //'first' is the float index
        //'second' is the content of the map at index 'first'. in this case 'second' is the map of ordinate values for analysis point 'first'

        //output the location of the analysis point
        influence_line_out << setprecision(4) << "\n" << (it->first) / this_bridge.length_span_one << setprecision(1) << "," << it->first;

        for(auto ordinate : (it->second)){
            influence_line_out << setprecision(4) << "," << ordinate.second;
        }
    }
}

/*void write_analysis_point_extremes(ofstream& analysis_point_extremes_out, map<float, analysis_point_extreme_set>& ap_extremes, bridge& this_bridge)
{
    //analysis_point_extremes_out << "\n";

    auto it = ap_extremes.begin();

    for(int i = 0; i < it->second.this_truck.num_axles; i++){
        analysis_point_extremes_out << "," << it->second.this_truck.axles[i].weight << "," << it->second.this_truck.axles[i].position;
    }

    analysis_point_extremes_out << "\nanalysis point ratio,analysis point,max moment,min moment,max shear,min shear\n";

    for(it; it != ap_extremes.end(); it++){
        analysis_point_extremes_out << setprecision(4) << (it->first) / this_bridge.length_span_one << "," << it->first << "," << it->second.max_moment << "," << it->second.min_moment << "," << it->second.max_shear << "," << it->second.min_shear << "\n";
    }
}*/

void write_truck(ofstream& truck_out, truck& this_truck)
{
    for(auto a = 0; a < this_truck.num_axles; a++){
        truck_out << "\n" << this_truck.number << "," << this_truck.num_axles << "," << a+1 << "," << this_truck.axles[a].weight << "," << this_truck.axles[a].position;
    }
}

void write_all_response(truck& this_truck, float analysis_point, float moment, float shear_left, float shear_right)
{

    all_response_out << "\n" << this_truck.number << "," << this_truck.direction << "," << this_truck.axles[0].position << "," << analysis_point << "," << moment << "," << shear_left << "," << shear_right;
}

void write_extreme_response(ofstream& extreme_response_out, map<float, extreme_set>& ap_extremes)
{
    for(auto it = ap_extremes.begin(); it != ap_extremes.end(); it++){
        float analysis_point = it->first;
        extreme_set this_set = it->second;

        //cout << "\nAP: " << analysis_point;

        extreme_response_out << "\n" << this_set.max_moment_truck.number << "," << this_set.max_moment_truck.direction << "," << this_set.max_moment_truck.axles[0].position << "," << analysis_point << "," << this_set.max_moment << "," << "NaN" << "," << "NaN";
        extreme_response_out << "\n" << this_set.min_moment_truck.number << "," << this_set.min_moment_truck.direction << "," << this_set.min_moment_truck.axles[0].position << "," << analysis_point << "," << this_set.min_moment << "," << "NaN" << "," << "NaN";

        extreme_response_out << "\n" << this_set.max_shear_left_truck.number << "," << this_set.max_shear_left_truck.direction << "," << this_set.max_shear_left_truck.axles[0].position << "," << analysis_point << "," << "NaN" << "," << this_set.max_shear_left << "," << "NaN";
        extreme_response_out << "\n" << this_set.min_shear_left_truck.number << "," << this_set.min_shear_left_truck.direction << "," << this_set.min_shear_left_truck.axles[0].position << "," << analysis_point << "," << "NaN" << "," << this_set.min_shear_left << "," << "NaN";

        extreme_response_out << "\n" << this_set.max_shear_right_truck.number << "," << this_set.max_shear_right_truck.direction << "," << this_set.max_shear_right_truck.axles[0].position << "," << analysis_point << "," << "NaN" << "," << "NaN" << "," << this_set.max_shear_right;
        extreme_response_out << "\n" << this_set.min_shear_right_truck.number << "," << this_set.min_shear_right_truck.direction << "," << this_set.min_shear_right_truck.axles[0].position << "," << analysis_point << "," << "NaN" << "," << "NaN" << "," << this_set.min_shear_right;
    }
}

