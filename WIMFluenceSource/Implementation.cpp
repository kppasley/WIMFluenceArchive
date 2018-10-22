/*
WIMFluence calculates shear and moment influence lines and the resulting shear and moment reactions based on weigh-in-motion data. Copyright © 2016-2018 Kenneth Pasley

This file is part of WIMFluence.

WIMFluence is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

WIMFluence is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with WIMFluence.  If not, see http://www.gnu.org/licenses.
*/

#include "Implementation.h"

truck hs20_truck()
{
    truck truck_name;

    truck_name.num_axles = 3;

    axle new_axle;
    truck_name.axles.push_back(new_axle);
    truck_name.axles.push_back(new_axle);
    truck_name.axles.push_back(new_axle);
    truck_name.axles[0].position = 0;
    truck_name.axles[1].position = -14;
    truck_name.axles[2].position = -28;
    truck_name.axles[0].weight = 8;
    truck_name.axles[1].weight = 32;
    truck_name.axles[2].weight = 32;

    return truck_name;
}

truck reverse_truck(truck truck_in)
{
    truck truck_out;
    axle new_axle;

    float length = truck_in.axles[0].position - (truck_in.axles[truck_in.num_axles - 1].position);

    truck_out.number = truck_in.number;
    truck_out.num_axles = truck_in.num_axles;

    for(int i = 0; i < truck_out.num_axles; i++){ //sets the new positions such that when the vector is reversed they will be correct
        truck_out.axles.push_back(new_axle);
        truck_out.axles[i].weight = truck_in.axles[i].weight;
        truck_out.axles[i].position = truck_in.axles[i].position * (-1) - length;
    }

    truck_out.direction = 'b';

    //reverse(truck_out.axles.begin(), truck_out.axles.end()); //reverses the vector.

    //print_truck(truck_in);
    //print_truck(truck_out);

    return truck_out;
}

void truck_forces(bridge& this_bridge, float analysis_point, truck& this_truck, float& truck_shear_left, float& truck_shear_right, float& truck_moment)//, ofstream& all_response_out)
{
    //truck_shear_left = 0;
    //truck_shear_right = 0;
    //truck_moment = 0;

    for(axle this_axle : this_truck.axles){
        float ordinate_location = this_axle.position;
        //ignore axles not on the bridge
        if((ordinate_location >= 0) && (ordinate_location <= this_bridge.length_total)){
            float weight = this_axle.weight;
            float length = this_bridge.length_span_one;
            truck_shear_left += weight * this_bridge.shear_ordinates_left[analysis_point][ordinate_location];
            truck_shear_right += weight * this_bridge.shear_ordinates_right[analysis_point][ordinate_location];
            truck_moment += weight * length * this_bridge.moment_ordinates[analysis_point][ordinate_location];
        }
    }
    /*if(truck_moment < 14.009 && truck_moment > 14.007){
        cout << "\n" << truck_moment << "\n";
    }*/

    //write_all_response(this_truck, analysis_point, truck_moment, truck_shear_left, truck_shear_right);
}

void truck_force_comparison(bridge& this_bridge, float analysis_point, truck& this_truck, extreme_set& this_ap_extreme_set)
{
    float current_shear_left = 0;
    float current_shear_right = 0;
    float current_moment = 0;

    //print_truck(forward_truck);
    truck_forces(this_bridge, analysis_point, this_truck, current_shear_left, current_shear_right, current_moment);

    //Compare with analysis point extremes
    if(current_shear_left > this_ap_extreme_set.max_shear_left){
        this_ap_extreme_set.max_shear_left = current_shear_left;
        this_ap_extreme_set.max_shear_left_truck = this_truck;
    }
    if(current_shear_left < this_ap_extreme_set.min_shear_left){
        this_ap_extreme_set.min_shear_left = current_shear_left;
        this_ap_extreme_set.min_shear_left_truck = this_truck;
    }
    if(current_shear_right > this_ap_extreme_set.max_shear_right){
        this_ap_extreme_set.max_shear_right = current_shear_right;
        this_ap_extreme_set.max_shear_right_truck = this_truck;
    }
    if(current_shear_right < this_ap_extreme_set.min_shear_right){
        this_ap_extreme_set.min_shear_right = current_shear_right;
        this_ap_extreme_set.min_shear_right_truck = this_truck;
    }
    if(current_moment > this_ap_extreme_set.max_moment){
        this_ap_extreme_set.max_moment = current_moment;
        this_ap_extreme_set.max_moment_truck = this_truck;
    }
    if(current_moment < this_ap_extreme_set.min_moment){
        this_ap_extreme_set.min_moment = current_moment;
        this_ap_extreme_set.min_moment_truck = this_truck;
    }

    return;
}

void iterate_truck(bridge& this_bridge, truck& forward_truck, map<float, extreme_set>& ap_extremes)
{
    truck reversed_truck = reverse_truck(forward_truck);

    extreme_set default_set;

    //initialize extremes for every analysis point
    //ap_extremes[0.0] = default_set; //initialize first set
    for(int i = 0; i < this_bridge.spans; i++){
        //for every 20th within the span
        for(float j = 1; j <= 20.0; j++){
            float analysis_point = this_bridge.support_positions[i];
            analysis_point += this_bridge.span_lengths[i] * j / 20.0;
            //ap_extremes[analysis_point] = default_set; //initialize extremes for every analysis point
        }
    }

    while(forward_truck.axles[forward_truck.num_axles - 1].position <= this_bridge.length_total){
        //perform calculation for first analysis point
        truck_force_comparison(this_bridge, 0.0, forward_truck, ap_extremes[0.0]);
        truck_force_comparison(this_bridge, 0.0, reversed_truck, ap_extremes[0.0]);

        //perform calculation for every analysis point
        for(int i = 0; i < this_bridge.spans; i++){
            //for every 20th within the span
            for(float j = 1; j <= 20.0; j++){
                float analysis_point = this_bridge.support_positions[i];
                analysis_point += this_bridge.span_lengths[i] * j / 20.0;

                //cout << "\nap:" << setw(7) << analysis_point;

                //calc forces and compare to extremes
                truck_force_comparison(this_bridge, analysis_point, forward_truck, ap_extremes[analysis_point]);
                truck_force_comparison(this_bridge, analysis_point, reversed_truck, ap_extremes[analysis_point]);
            }
        }

        //Move trucks forward
        for(int i = 0; i < forward_truck.num_axles; i++){
            //move the truck forward by 1 ft
            forward_truck.axles[i].position = forward_truck.axles[i].position + 1.0;
            reversed_truck.axles[i].position = reversed_truck.axles[i].position + 1.0;
        }
    }
    return;
}

//---------------------------
//Simple span influence ordinate
//---------------------------
float simple_span(bridge& this_bridge, float ordinate_location, float analysis_point_location)
{
    float length_opposite = 0; //length of bridge on the opposite side of the analysis point from the ordinate point
    float custom_ordinate_location = 0; //location of the ordinate value. allows the use of a reversed coordinate system (xi instead of x)
    float ordinate_value = 0;

    if(ordinate_location <= analysis_point_location){
        length_opposite = (this_bridge.length_total - analysis_point_location);
        custom_ordinate_location = ordinate_location;
    }
    else if(ordinate_location > analysis_point_location){
        length_opposite = analysis_point_location;
        custom_ordinate_location = (this_bridge.length_total - ordinate_location);
    }

    ordinate_value = (length_opposite / this_bridge.length_total) * custom_ordinate_location;
    ordinate_value = ordinate_value;// / this_bridge.length_span_one;

    return ordinate_value;
}

//---------------------------
//Displacement at the ordinate location given a displacement at the analysis point location
//---------------------------
float displacement(bridge& this_bridge, float ordinate_location, float analysis_point_location)
{
    float length_opposite = 0; //length of bridge on the opposite side of the analysis point from the ordinate point
    float custom_ordinate_location = 0; //location of the ordinate value. allows the use of a reversed coordinate system (xi instead of x)
    float displacement_value = 0;

    if(ordinate_location <= analysis_point_location){
        length_opposite = (this_bridge.length_total - analysis_point_location);
        custom_ordinate_location = ordinate_location;
    }
    else if(ordinate_location > analysis_point_location){
        length_opposite = analysis_point_location;
        custom_ordinate_location = (this_bridge.length_total - ordinate_location);
    }

    //The modulus of elasticity and moment of inertia are treated as 1 since they come out in the wash
    displacement_value = (length_opposite * custom_ordinate_location *
                         (pow(this_bridge.length_total,2) - pow(length_opposite,2) - pow(custom_ordinate_location,2)))
                         / (6 * this_bridge.length_total); // Lo*x*(L^2-Lo^2-x^2)/(6*L)

    return displacement_value;
}

//---------------------------
//The ratio of displacements at the ordinate location and the analysis point location due to a displacement at the a.p.
//Makes the displacement (ratio) at the a.p. equal to 1
//---------------------------
float displacement_ratio(bridge& this_bridge, float ordinate_location, float analysis_point_location)
{
    float displacement_ratio_value = 0;

    displacement_ratio_value = displacement(this_bridge, ordinate_location, analysis_point_location) / displacement(this_bridge, analysis_point_location, analysis_point_location); //a_p_l in both places is intentional

    return displacement_ratio_value;
}

//---------------------------
//Set of simple span ordinates for every combination of two internal supports
//---------------------------
void simple_set(bridge& this_bridge){
    int n = this_bridge.support_positions.size()-1;
    for(int i = 1; i < n; i++){
        vector<float> temp;
        float a = this_bridge.support_positions[i];
        for(int j = 1; j < n; j++){
            float b = this_bridge.support_positions[j];
            temp.push_back(simple_span(this_bridge, b, a));

        }
        this_bridge.support_simple_set.push_back(temp);
    }

}

//---------------------------
//Displacement ratio set for every combination of two internal supports
//---------------------------
void delta_set(bridge& this_bridge){
    int n = this_bridge.support_positions.size()-1;
    for(int i = 1; i < n; i++){
        vector<float> temp;
        float a = this_bridge.support_positions[i];
        for(int j = 1; j < n; j++){
            float b = this_bridge.support_positions[j];
            temp.push_back(displacement_ratio(this_bridge, b, a));
        }
        this_bridge.support_delta_set.push_back(temp);
    }
}

//---------------------------
//Multiply a row in a matrix by a scalar
//---------------------------
void row_multiply(vector< vector<float> >& matrix, int row, float factor){
    int n = matrix.size();

    for(int i = 0; i <= n - 1; i++){
        matrix[i][row] = matrix[i][row] * factor;
    }
}

//---------------------------
//Add the contents of one row of a matrix to another row
//---------------------------
void row_add_row(vector< vector<float> >& matrix, int row1, int row2){
    //add row 2 to row 1
    int n = matrix.size();

    for(int i = 0; i <= n-1; i++){
        matrix[i][row1] = matrix[i][row1] + matrix[i][row2];
    }
}

//---------------------------
//Perform Gaussian elimination to solve for the factors applied to the displacement set
//---------------------------
vector< vector<float> > gaussian_elimination(vector< vector<float> > matrix)
{
    int m = matrix[0].size();
    //int n = matrix.size();

    //lower zeros
    for(int a = 0; a < m - 1; a++){
        for(int b = a+1; b <= m - 1; b++){
            float factor = - 1 * matrix[a][a] / matrix[a][b];

            row_multiply(matrix, b, factor);
            row_add_row(matrix, b, a);
        }
    }

    //upper zeros
    for(int a = 1; a < m; a++){
        for(int b = 0; b < a; b++){
            float factor = -1 * matrix [a][a] / matrix [a][b];

            row_multiply(matrix, b, factor);
            row_add_row(matrix, b, a);
        }
    }

    //diagonal ones
    for(int a = 0; a < m; a++){
        float factor = 1 / matrix[a][a];

        row_multiply(matrix, a, factor);
    }

    return matrix;
}

//---------------------------
//Solve for the factors to be applied to the displacement ratios at the supports
//---------------------------
void solve_factors(bridge& this_bridge)
{
    //set simple set to negative to solve systems of linear equations
    for(int i = 0; i <= this_bridge.support_simple_set.size() - 1; i++){
        for(int j = 0; j <= this_bridge.support_simple_set.size() - 1; j++){
            this_bridge.support_simple_set[i][j] = (-1) * this_bridge.support_simple_set[i][j];
        }
    }

    //Solve systems of linear equations to find the factors
    for(int i = 0; i < this_bridge.spans - 1; i++){

        //Initialize the matrix for solving
        vector< vector<float> > solve_set = this_bridge.support_delta_set;
        //Attach the simple set to the solve set. This is the set of constants in the system of linear equations
        solve_set.push_back(this_bridge.support_simple_set[i]);
        //Initialize the solved matrix as the return of gaussian elimination on the matrix for solving
        vector< vector<float> > solved_set = gaussian_elimination(solve_set);
        //Attach the
        this_bridge.support_factors.push_back(solved_set[this_bridge.spans - 1]);
    }

    //return this_bridge.support_factors;
}

//---------------------------
//Moment ordinates for all supports given a unit load at a location
//---------------------------
void moment_supports(bridge& this_bridge, float ordinate_location)
{
    int n = this_bridge.spans - 1;

    for(int i = 0; i < n; i++){
        float ordinate_value = simple_span(this_bridge, ordinate_location, this_bridge.support_positions[i+1]);

        for(int j = 0; j < n; j++){
            ordinate_value += displacement_ratio(this_bridge, ordinate_location, this_bridge.support_positions[j+1]) * this_bridge.support_factors[i][j];
        }
        this_bridge.support_ordinates[ordinate_location].push_back(ordinate_value);
    }
    this_bridge.support_ordinates[ordinate_location].push_back(0); //pushes a zero for the moment at the second external support
    auto it = this_bridge.support_ordinates[ordinate_location].begin(); //initializes the index for the following function as the beginning of the vector
    this_bridge.support_ordinates[ordinate_location].insert(it, 0); //inserts a zero for the moment at the first external support
}

//---------------------------
//Moment ordinates (multi-span)
//---------------------------
void moment_ordinates(bridge& this_bridge, float ordinate_location, float analysis_point_location)
{
    int n = this_bridge.span_lengths.size();

    for(int i = 0; i < n; i++){ //i is the index of the support at the left end of a span
        int j = i + 1; //index of support at the right end of a span

        if(analysis_point_location == this_bridge.support_positions[i]){
            //if the analysis point is at the left support, set the moment influence ordinate equal to the corresponding one in the supports map
            this_bridge.moment_ordinates[analysis_point_location][ordinate_location] = this_bridge.support_ordinates[ordinate_location][i];
            break;
        }
        else if(analysis_point_location == this_bridge.support_positions[j]){
            //if the analysis point is at the right support, set the moment influence ordinate equal to the corresponding one in the supports map
            this_bridge.moment_ordinates[analysis_point_location][ordinate_location] = this_bridge.support_ordinates[ordinate_location][j];
            break;
        }
        else if((analysis_point_location > this_bridge.support_positions[i]) && (analysis_point_location < this_bridge.support_positions[j])){
            //(above if statement) check to see if the analysis point is within this span. if it isn't, move on to the next span in the next for-loop iteration
            //position of the analysis point within this span
            float x = analysis_point_location - this_bridge.support_positions[i];
            //length of this span
            float L = this_bridge.span_lengths[i];
            //determine the moments at the left and right ends of the span
            //cout << "\n" << __LINE__ << "\t" << ordinate_location << "\n";
            float left_moment = this_bridge.support_ordinates[ordinate_location][i];
            float right_moment = this_bridge.support_ordinates[ordinate_location][j];

            //check to see if the ordinate point is located within this span as well
            //this determines if the point load comes into play in the calculations
            if((ordinate_location > this_bridge.support_positions[i]) && (ordinate_location < this_bridge.support_positions[j])){
                //check which side of the ordinate location the analysis point is on
                if(analysis_point_location <= ordinate_location){
                    float b = this_bridge.support_positions[j] - ordinate_location;
                    this_bridge.moment_ordinates[analysis_point_location][ordinate_location] = (b / L + (right_moment - left_moment) / L) * x  + left_moment;
                    break;
                }
                else if(analysis_point_location > ordinate_location){
                    float a = ordinate_location - this_bridge.support_positions[i];
                    this_bridge.moment_ordinates[analysis_point_location][ordinate_location] = (a / L - (right_moment - left_moment) / L) * (L - x)  + right_moment;
                    break;
                }
            }
            else{
                this_bridge.moment_ordinates[analysis_point_location][ordinate_location] = ((right_moment - left_moment) / L) * x + left_moment;
            }
        }
    }
    //make the ordinate in terms of the length of span 1.
    this_bridge.moment_ordinates[analysis_point_location][ordinate_location] = this_bridge.moment_ordinates[analysis_point_location][ordinate_location] / this_bridge.length_span_one;
    return;
}

//---------------------------
//Shear ordinates (multi-span)
//---------------------------
void shear_ordinates(bridge& this_bridge, float ordinate_location, float analysis_point_location)
{
    //
    if(analysis_point_location == 0){
        this_bridge.shear_ordinates_left[analysis_point_location][ordinate_location] = 0;
        float b = 0;
        float L = this_bridge.span_lengths[0];
        float left_moment = this_bridge.support_ordinates[ordinate_location][0];
        float right_moment = this_bridge.support_ordinates[ordinate_location][1];
        if((ordinate_location < this_bridge.support_positions[1]) && (ordinate_location >= 0)){
            b = this_bridge.support_positions[1] - ordinate_location;
        }
        this_bridge.shear_ordinates_right[analysis_point_location][ordinate_location] = b / L + (right_moment - left_moment) / L;
        return;
    }
    //
    else if(analysis_point_location == this_bridge.length_total){
        this_bridge.shear_ordinates_right[analysis_point_location][ordinate_location] = 0;
        int last_support = this_bridge.span_lengths.size();
        float a = 0;
        float L = this_bridge.span_lengths[last_support - 1];
        float left_moment = this_bridge.support_ordinates[ordinate_location][last_support - 1];
        float right_moment = this_bridge.support_ordinates[ordinate_location][last_support];
        if((ordinate_location <= this_bridge.length_total) && (ordinate_location > this_bridge.support_positions[last_support - 1])){
            a = ordinate_location - this_bridge.support_positions[last_support - 1];
        }
        this_bridge.shear_ordinates_left[analysis_point_location][ordinate_location] = (right_moment - left_moment) / L - a / L;
        return;
    }
    //
    else{
        int n = this_bridge.span_lengths.size();
        for(int i = 0; i < n; i++){
            //i is the first support of the relevant span
            //j is the second
            int j = i + 1;
            //check to see if the a.p. falls on the second support. It will never fall on the first as that is taken care of above
            if(analysis_point_location == this_bridge.support_positions[j]){
                float a = 0;
                float b = 0;
                float left_moment = this_bridge.support_ordinates[ordinate_location][i];
                float right_moment = this_bridge.support_ordinates[ordinate_location][j];
                float L = this_bridge.span_lengths[i];
                //calculate the left shear using this span
                if((ordinate_location > this_bridge.support_positions[i]) && (ordinate_location <= this_bridge.support_positions[j])){
                    a = ordinate_location - this_bridge.support_positions[i];
                }
                this_bridge.shear_ordinates_left[analysis_point_location][ordinate_location] = (right_moment - left_moment) / L - a / L;

                left_moment = this_bridge.support_ordinates[ordinate_location][j];
                right_moment = this_bridge.support_ordinates[ordinate_location][j+1];
                L = this_bridge.span_lengths[j];
                //calculate the right shear using the next span
                if((ordinate_location >= this_bridge.support_positions[j]) && (ordinate_location < this_bridge.support_positions[j+1])){
                    b = this_bridge.support_positions[j+1] - ordinate_location;
                }
                this_bridge.shear_ordinates_right[analysis_point_location][ordinate_location] = b / L + (right_moment - left_moment) / L;
                return;
            }
            //check to see if the a.p. falls within this span
            if((analysis_point_location > this_bridge.support_positions[i]) && (analysis_point_location < this_bridge.support_positions[j])){
                float L = this_bridge.span_lengths[i];
                float left_moment = this_bridge.support_ordinates[ordinate_location][i];
                float right_moment = this_bridge.support_ordinates[ordinate_location][j];
                float a = 0;
                float b = 0;
                if(ordinate_location == analysis_point_location){
                    a = ordinate_location - this_bridge.support_positions[i];
                    b = this_bridge.support_positions[j] - ordinate_location;
                    this_bridge.shear_ordinates_left[analysis_point_location][ordinate_location] = (right_moment - left_moment) / L - a / L;
                    this_bridge.shear_ordinates_right[analysis_point_location][ordinate_location] = b / L + (right_moment - left_moment) / L;
                    return;
                }
                else if((ordinate_location < analysis_point_location) && (ordinate_location > this_bridge.support_positions[i])){
                    a = ordinate_location - this_bridge.support_positions[i];
                    this_bridge.shear_ordinates_left[analysis_point_location][ordinate_location] = (right_moment - left_moment) / L - a / L;
                    this_bridge.shear_ordinates_right[analysis_point_location][ordinate_location] = (right_moment - left_moment) / L - a / L;
                    return;
                }
                else if((ordinate_location < this_bridge.support_positions[j]) && (ordinate_location > analysis_point_location)){
                    b = this_bridge.support_positions[j] - ordinate_location;
                    this_bridge.shear_ordinates_left[analysis_point_location][ordinate_location] = b / L + (right_moment - left_moment) / L;
                    this_bridge.shear_ordinates_right[analysis_point_location][ordinate_location] = b / L + (right_moment - left_moment) / L;
                    return;
                }
                else{
                    this_bridge.shear_ordinates_left[analysis_point_location][ordinate_location] = b / L + (right_moment - left_moment) / L;
                    this_bridge.shear_ordinates_right[analysis_point_location][ordinate_location] = b / L + (right_moment - left_moment) / L;
                    return;
                }
            }
        }
    }
}
