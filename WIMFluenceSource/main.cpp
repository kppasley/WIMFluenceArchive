/*
WIMFluence calculates shear and moment influence lines and the resulting shear and moment reactions based on weigh-in-motion data. Copyright © 2016-2018 Kenneth Pasley

This file is part of WIMFluence.

WIMFluence is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

WIMFluence is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with WIMFluence.  If not, see http://www.gnu.org/licenses.
*/

#include <time.h>
#include "Implementation.h" //implementation functions
#include "Inputs.h"         //input reading functions
#include "Prints.h"         //print to console functions
#include "Outputs.h"        //write to file functions
#include "iostream"
#include "iomanip"
#include <string>

using std::cin;
using std::cout;
using std::setprecision;
using std::fixed;
using std::setw;
using std::right;


bool write_all_response_values = FALSE;

ofstream all_response_out;

int main()
{

    bool write_original_files = false;


    //file variables
    string wim_file_name; //needed for the names of some output files
    ifstream wim_in;
    ofstream influence_line_out;
    ofstream truck_out;
    ofstream analysis_point_extremes_out;
    ofstream shear_out;
    ofstream moment_out;
    ofstream extreme_response_out;


    //TEST
    //analysis_point_extreme_set test_set;
    //cout << "ap extreme default max moment:" << test_set.max_moment;
    //cout << "ap extreme default min moment:" << test_set.min_moment;
    //cout << "ap extreme default max shear:" << test_set.max_shear;
    //cout << "ap extreme default min shear:" << test_set.min_shear;


    //variable initializations
    bridge this_bridge; //initialize bridge struct
    string line;

    open_input_wim_file(wim_in, wim_file_name);


    //are trucks in US units?
    string units_bool = "a";
    while(units_bool != "y" && units_bool != "n"){
        cout << "\nAre the truck parameters in metric (decimeters and 100 kilograms)? y/n\n";
        cin >> units_bool;
        //5cin.clear();
        //cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }


    get_bridge(this_bridge);

    //These file openings are done later than the first since these need the first and the bridge
    open_output_influence_line_file(influence_line_out, this_bridge);
    open_output_truck_file(truck_out, wim_file_name, this_bridge);
    if(write_all_response_values){
        open_output_all_response_file(all_response_out, wim_file_name, this_bridge);
    }
    open_output_extreme_response_file(extreme_response_out, wim_file_name, this_bridge);
    if(write_original_files){
        open_output_shear_file(shear_out, wim_file_name, this_bridge);
        open_output_moment_file(moment_out, wim_file_name, this_bridge);
        open_output_analysis_point_extremes_file(analysis_point_extremes_out, wim_file_name, this_bridge);
    }


    print_bridge(this_bridge);

    write_bridge_info(this_bridge, analysis_point_extremes_out);
    if(write_all_response_values){
        write_bridge_info(this_bridge, all_response_out);
    }
    write_bridge_info(this_bridge, extreme_response_out);


    truck_out << "truck_index,num_axles,axle_num,axle_weight,axle_rel_pos";
    if(write_all_response_values){
        all_response_out << "\n\ntruck_index,truck_direction,first_axle_pos,analysis_point,moment,shear_left,shear_right";
    }
    extreme_response_out << "\n\ntruck_index,truck_direction,first_axle_pos,analysis_point,moment,shear_left,shear_right";
    moment_out << "max_moment,location,truck,,,min_moment,location,truck\n";
    shear_out << "max_shear,location,truck,,,min_shear,location,truck\n";

    //start clock
    clock_t tStart = clock();

    //preliminary calculations
    if(this_bridge.spans > 1){
        simple_set(this_bridge);
        delta_set(this_bridge);
        solve_factors(this_bridge);
    }

    //find influence lines
    //for every foot load position interval
    for(float position = 0; position <= this_bridge.length_total; position += 1.0){
        moment_supports(this_bridge, position);
        moment_ordinates(this_bridge, position, 0);
        shear_ordinates(this_bridge, position, 0);

        //for every span
        for(int i = 0; i < this_bridge.spans; i++){

            //for every 20th within the span
            for(float j = 1; j <= 20; j++){
                float analysis_point = this_bridge.support_positions[i];
                analysis_point += this_bridge.span_lengths[i] * j / 20;

                moment_ordinates(this_bridge, position, analysis_point);
                shear_ordinates(this_bridge, position, analysis_point);

                //cout << "\nspan:\t" << i+1 << "\tposition:\t" << position << "\tap:\t" << analysis_point;
            }
        }
    }

    write_influence_line1(this_bridge, influence_line_out);
    influence_line_out.close();

    map<float, extreme_set> ap_extremes;

    int truck_count = 0;
    while(getline(wim_in, line)){
        truck_count++;

        extreme_set extremes;

        truck current_truck = get_truck(line, units_bool);
        current_truck.number = truck_count;

        //write_truck
        write_truck(truck_out, current_truck);

        analysis_point_extremes_out << "\n\ntruck:," << current_truck.num_axles << " axles";
        for(int i = 0; i < current_truck.num_axles; i++){
            //printf("%d\n", current_truck.num_axles);
            analysis_point_extremes_out << "," << current_truck.axles[i].weight << "," << current_truck.axles[i].position;
        }

        //analysis_point_extremes_out << "\n"
        //print_truck(current_truck);

        iterate_truck(this_bridge, current_truck, ap_extremes);

        /*if(write_original_files){
            write_shear2(extremes, shear_out);
            write_moment2(extremes, moment_out);
            write_analysis_point_extremes(analysis_point_extremes_out, ap_extremes, this_bridge);
        }*/
    }

    write_extreme_response(extreme_response_out, ap_extremes);

    //close files
    wim_in.close();
    //influence_line_out.close();
    truck_out.close();
    all_response_out.close();
    extreme_response_out.close();
    /*if(write_original_files){
        shear_out.close();
        moment_out.close();
        analysis_point_extremes_out.close();
    }*/

    //output run duration
    printf("\n\nTime taken: %.2fs\n", (float)(clock() - tStart)/CLOCKS_PER_SEC);

    //wait
    getchar();
    getchar();

    return 0;
}
