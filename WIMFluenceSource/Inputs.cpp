/*
WIMFluence calculates shear and moment influence lines and the resulting shear and moment reactions based on weigh-in-motion data. Copyright © 2016-2018 Kenneth Pasley

This file is part of WIMFluence.

WIMFluence is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

WIMFluence is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with WIMFluence.  If not, see http://www.gnu.org/licenses.
*/

#include "Inputs.h"

bool file_exists(const std::string& file) {
    struct stat buffer;
    return (stat(file.c_str(), &buffer) == 0);
}

void open_input_wim_file(ifstream &wim_in, string& wim_file_name)
{
    if (CreateDirectory("input", NULL))
    {
        cout << "Folder 'input' has been created.\n\n";
    }
    else if (ERROR_ALREADY_EXISTS == GetLastError())
    {
        // Directory already exists
    }
    else
    {
        cout << "\nFailed at line " << __LINE__ << " of Inputs.cpp\n";
    }

    if (CreateDirectory("output", NULL))
    {
        cout << "Folder 'output' has been created.\n\n";
    }
    else if (ERROR_ALREADY_EXISTS == GetLastError())
    {
        // Directory already exists
    }
    else
    {
        cout << "\nFailed at line " << __LINE__ << " of Inputs.cpp\n";
    }

    cout << "Input the name of the WIM input file in 'input' folder: " << endl;
    cin >> wim_file_name;

    while(! file_exists("input/" + wim_file_name))
    {
        cout << "File not found. Please enter the name again. (Include file extension.)" << endl;
        cin >> wim_file_name;
    }
    wim_in.open(("input/" + wim_file_name).c_str());

    string placeholder;
    getline(wim_in, placeholder);//skips the first line since it is data labels

    wim_file_name.erase(wim_file_name.find_last_of("."), string::npos); //remove the extension of the filename so that it can easily be used by other functions
}

void open_output_shear_file(ofstream &shear_out, string wim_file_name, bridge& this_bridge)
{
    stringstream bridge_folder_stream;
    bridge_folder_stream << this_bridge.spans << "spans";
    for(float length : this_bridge.span_lengths){
        bridge_folder_stream << "_" << length;
    }
    string bridge_folder = "output/" + bridge_folder_stream.str();

    stringstream shear_file;
    shear_file << wim_file_name << "_S_";
    shear_file << this_bridge.spans << "spans";
    for(float length : this_bridge.span_lengths){
        shear_file << "_" << length;
    }
    shear_file << ".csv";
    cout << "\n" << shear_file.str().c_str() << "\n";
    shear_out.open((bridge_folder + "/" + shear_file.str()).c_str());
}

void open_output_moment_file(ofstream &moment_out, string wim_file_name, bridge& this_bridge)
{
    stringstream bridge_folder_stream;
    bridge_folder_stream << this_bridge.spans << "spans";
    for(float length : this_bridge.span_lengths){
        bridge_folder_stream << "_" << length;
    }
    string bridge_folder = "output/" + bridge_folder_stream.str();

    stringstream moment_file;
    moment_file << wim_file_name << "_M_";
    moment_file << this_bridge.spans << "spans";
    for(float length : this_bridge.span_lengths){
        moment_file << "_" << length;
    }
    moment_file << ".csv";
    cout << "\n" << moment_file.str().c_str() << "\n";
    moment_out.open((bridge_folder + "/" + moment_file.str()).c_str());
}

void open_output_influence_line_file(ofstream &influence_line_out, bridge& this_bridge)
{
    stringstream bridge_folder_stream;
    bridge_folder_stream << this_bridge.spans << "spans";
    for(float length : this_bridge.span_lengths){
        bridge_folder_stream << "_" << length;
    }
    string bridge_folder = "output/" + bridge_folder_stream.str();

    if (CreateDirectory(bridge_folder.c_str(), NULL))
    {
        cout << "Folder '" << bridge_folder << "' has been created.\n\n";
    }
    else if (ERROR_ALREADY_EXISTS == GetLastError())
    {
        // Directory already exists
    }
    else
    {
        cout << "\nFailed at line " << __LINE__ << " of Inputs.cpp\n";
    }

    stringstream il_file;
    il_file << "IL_";
    il_file << this_bridge.spans << "spans";
    for(float length : this_bridge.span_lengths){
        il_file << "_" << length;
    }
    il_file << ".csv";
    cout << "\n" << il_file.str().c_str() << "\n";
    influence_line_out.open((bridge_folder + "/" + il_file.str()).c_str());
}

void open_output_analysis_point_extremes_file(ofstream& analysis_point_extremes_out, string wim_file_name, bridge& this_bridge)
{
    stringstream bridge_folder_stream;
    bridge_folder_stream << this_bridge.spans << "spans";
    for(float length : this_bridge.span_lengths){
        bridge_folder_stream << "_" << length;
    }
    string bridge_folder = "output/" + bridge_folder_stream.str();

    stringstream ap_file;
    ap_file << wim_file_name << "_AP_";
    ap_file << this_bridge.spans << "spans";
    for(float length : this_bridge.span_lengths){
        ap_file << "_" << length;
    }
    ap_file << ".csv";
    cout << "\n" << ap_file.str().c_str() << "\n";
    analysis_point_extremes_out.open((bridge_folder + "/" + ap_file.str()).c_str());
}

void open_output_truck_file(ofstream& truck_out, string wim_file_name, bridge& this_bridge)
{
    stringstream bridge_folder_stream;
    bridge_folder_stream << this_bridge.spans << "spans";
    for(float length : this_bridge.span_lengths){
        bridge_folder_stream << "_" << length;
    }
    string bridge_folder = "output/" + bridge_folder_stream.str();

    stringstream truck_file;
    truck_file << wim_file_name << "_formatted.csv";
    cout << "\n" << truck_file.str().c_str() << "\n";
    truck_out.open((bridge_folder + "/" + truck_file.str()).c_str());
}

void open_output_all_response_file(ofstream& all_response_out, string wim_file_name, bridge& this_bridge)
{
    stringstream bridge_folder_stream;
    bridge_folder_stream << this_bridge.spans << "spans";
    for(float length : this_bridge.span_lengths){
        bridge_folder_stream << "_" << length;
    }
    string bridge_folder = "output/" + bridge_folder_stream.str();

    stringstream all_response_file;
    all_response_file << wim_file_name << "_all_response_";
    all_response_file << this_bridge.spans << "spans";
    for(float length : this_bridge.span_lengths){
        all_response_file << "_" << length;
    }
    all_response_file << ".csv";
    cout << "\n" << all_response_file.str().c_str() << "\n";
    all_response_out.open((bridge_folder + "/" + all_response_file.str()).c_str());
}

void open_output_extreme_response_file(ofstream& extreme_response_out, string wim_file_name, bridge& this_bridge)
{
    stringstream bridge_folder_stream;
    bridge_folder_stream << this_bridge.spans << "spans";
    for(float length : this_bridge.span_lengths){
        bridge_folder_stream << "_" << length;
    }
    string bridge_folder = "output/" + bridge_folder_stream.str();

    stringstream extreme_response_file;
    extreme_response_file << wim_file_name << "_extreme_response_";
    extreme_response_file << this_bridge.spans << "spans";
    for(float length : this_bridge.span_lengths){
        extreme_response_file << "_" << length;
    }
    extreme_response_file << ".csv";
    cout << "\n" << extreme_response_file.str().c_str() << "\n";
    extreme_response_out.open((bridge_folder + "/" + extreme_response_file.str()).c_str());
}

void get_bridge(bridge& new_bridge)
{
    //Ratios are in terms of the length of span 1

    //Input the number of spans
    new_bridge.spans = 0;
    //cout << "\nspans: " << new_bridge.spans << "\n";
    cout << "\nInput the number of spans:\n";
    cin >> new_bridge.spans;
    cout << "\nspans: " << new_bridge.spans << "\n";

    //Input the lengths
    /*if (new_bridge.spans == 1){
        //Input the length of the SINGLE span
        cout << "\nInput the length of the span:\n";
        cin >> new_bridge.length_span_one;
        new_bridge.span_length_ratios.push_back(1);
        //Set the total length ratio to 1
        new_bridge.length_total_ratio = 1;
    }
    else{*/
        new_bridge.support_positions.push_back(0);
        //Input the length of the FIRST span
        cout << "\nInput the length of span 1:\n";
        cin >> new_bridge.length_span_one;
        //new_bridge.span_length_ratios.push_back(1);
        new_bridge.span_lengths.push_back(new_bridge.length_span_one);
        //Set the total length ratio to 1 for addition of the other spans later
        //new_bridge.length_total_ratio = 1;
        new_bridge.length_total = new_bridge.length_span_one;
        //Set the position ratio of the first internal support to 1
        //new_bridge.support_positions.push_back(1);
        new_bridge.support_positions.push_back(new_bridge.length_span_one);

        //Input the length of subsequent spans
        //Determine the total length ratio
        //Determine the locations of the internal supports (ratios)
        for(int i = 1; i <= (new_bridge.spans - 1); i++){
            float length = 0;
            cout << "\nInput the length of span " << (i + 1) << ":\n";
            cin >> length;

            //Determine the ratio
            //float new_span_ratio = (length / new_bridge.length_span_one);
            //new_bridge.span_length_ratios.push_back(new_span_ratio);
            new_bridge.span_lengths.push_back(length);

            //Extend the total length ratio
            //new_bridge.length_total_ratio += new_span_ratio;
            new_bridge.length_total += length;

            //Determine the locations of the internal supports
            //(Distance from initial external support to the relevant internal support)
            //The IF statement prevents the final exterior support from being included
            //if(i != (new_bridge.spans - 1)){
                new_bridge.support_positions.push_back(new_bridge.length_total);
            //}
        }
    //}
}

truck get_truck(string line, string units_bool)
{
    truck truck_out;
    //float position = 0;
    float space = 0;
    string holder_string;
    axle holder_axle;

    stringstream line_stream(line);
    line_stream >> truck_out.num_axles;
    //printf("Number of axles: %d\n", truck_out.num_axles);
    truck_out.axles.push_back(holder_axle);
    line_stream >> truck_out.axles[0].weight;
    truck_out.axles[0].position = 0;
    for(int i = 1; i < truck_out.num_axles; i++){
        line_stream >> space;
        holder_axle.position = truck_out.axles[i-1].position - space;
        line_stream >> holder_axle.weight;
        truck_out.axles.push_back(holder_axle);
    }

    if(units_bool == "y"){
        //round dm to nearest half ft
        //and convert 100 kilograms to pounds to kips
        for(int i = 0; i < truck_out.num_axles; i++){
            //divided by 3.048 to convert to feet; multiplied by 2 to allow rounding to the nearest half foot
            truck_out.axles[i].position = round(truck_out.axles[i].position / 3.048 * 2.0) / 2.0;
            //multiplied by 2.20462*100 to convert 100 kilograms to pounds to kips
            truck_out.axles[i].weight = truck_out.axles[i].weight * 2.20462 * 100 / 1000;
        }
    }


    return truck_out;
}

