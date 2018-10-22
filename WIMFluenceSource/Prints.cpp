/*
WIMFluence calculates shear and moment influence lines and the resulting shear and moment reactions based on weigh-in-motion data. Copyright © 2016-2018 Kenneth Pasley

This file is part of WIMFluence.

WIMFluence is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

WIMFluence is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with WIMFluence.  If not, see http://www.gnu.org/licenses.
*/

#include "Prints.h"

void print_truck(truck& this_truck)
{
    cout << "======================" << endl;
    //cout << "Number of axles: " << this_truck.num_axles << endl;
    //cout << "-------------------\n";
    cout << right << setw(4) << "Axle" << setw(10) << "Position" << setw(9) << "Weight\n";
    for(int i = 0; i < this_truck.num_axles; i++){
        //cout << "-------------------" << endl;
        //cout << "Axle:     " << i+1 << endl;
        //cout << "Position: " << this_truck.axles[i].position << endl;
        //cout << "Weight:   " << this_truck.axles[i].weight << endl;
        cout << right << setw(4) << i+1 << setw(10) << this_truck.axles[i].position << setw(8) << this_truck.axles[i].weight << "\n";
    }
    return;
}

/*void print_extreme_set(extreme_set& extremes)
{
    cout << "\n\n====================================" << endl;
    cout << "Max Shear:" << setw(12) << extremes.max_shear << endl;
    cout << "Analysis Point:" << setw(7) << extremes.max_shear_analysis_point << endl;
    //cout << "------------------------" << endl;
    print_truck(extremes.max_shear_truck);

    cout << "====================================" << endl;
    cout << "Min Shear:" << setw(12) << extremes.min_shear << endl;
    cout << "Analysis Point:" << setw(7) << extremes.min_shear_analysis_point << endl;
    //cout << "------------------------" << endl;
    print_truck(extremes.min_shear_truck);

    cout << "====================================" << endl;
    cout << "Max Moment:" << setw(11) << extremes.max_moment << endl;
    cout << "Analysis Point:" << setw(7) << extremes.max_moment_analysis_point << endl;
    //cout << "------------------------" << endl;
    print_truck(extremes.max_moment_truck);

    cout << "====================================" << endl;
    cout << "Min Moment:" << setw(11) << extremes.min_moment << endl;
    cout << "Analysis Point:" << setw(7) << extremes.min_moment_analysis_point << endl;
    //cout << "------------------------" << endl;
    print_truck(extremes.min_moment_truck);
}*/

void print_bridge(bridge& this_bridge)
{
    cout << "\nNumber of Spans:" << setw(20) << this_bridge.spans;
    cout << "\nLength of Span 1:" << setw(19) << this_bridge.length_span_one;
    cout << "\nTotal Length Ratio:" << setw(17) << this_bridge.length_total / this_bridge.length_span_one;
    //Print lengths of each span
    for(int i = 0; i <= (this_bridge.spans - 1); i++){
        cout << "\nLength Ratio of Span " << (i + 1) << ":" << setw(13) << this_bridge.span_lengths[i] / this_bridge.length_span_one;
    }
    for(int i = 0; i <= (this_bridge.spans); i++){
        //cout << "\n" << __LINE__ << " " << i << "\n";
        cout << "\nPosition Ratio of Support " << (i + 1) << ":" << setw(8) << this_bridge.support_positions[i] / this_bridge.length_span_one;
    }
    cout << "\n\n";
}

void print_matrix(vector< vector<float> >& matrix){
    int m = matrix[0].size();
    int n = matrix.size();
    cout << "\nm = " << m;
    cout << "\nn = " << n << "\n";

    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            cout /*<< setprecision(5)*/ << fixed << setw(10) << matrix[j][i];
        }
        cout << "\n";
    }
    cout << "\n"; //<< scientific;
}

void print_vector(vector<float>& matrix){
    int m = matrix.size();

    cout << "\nm = " << m;
    for(int i = 0; i < m; i++){
        cout << fixed << setw(10) /*<< setprecision(5)*/ << matrix[i];
    }
}
