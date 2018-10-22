# WIMFluence calculates shear and moment influence lines and the resulting shear and moment reactions based on weigh-in-motion data. Copyright © 2016-2018 Kenneth Pasley

# This file is part of WIMFluence.

# WIMFluence is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

# WIMFluence is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

# You should have received a copy of the GNU General Public License along with WIMFluence.  If not, see http://www.gnu.org/licenses.

import os
import winsound as ws
#import numpy as np
import pandas as pd
#import matplotlib.pyplot as plt
#from matplotlib.ticker import FixedLocator
#import timeit
#import sys
#import time

#control is the beginning of the filename for the control set of trucks
control_set_name = 'ArDOT'

max_plot_ratio = 2.0
offset_neglect = 3 #index of analysis points away from supports to neglect in moment graphs

#start_time = timeit.default_timer()

rootdir = os.getcwd()
print(rootdir)

#ap_ratios = [val / 20 for val in list(range(121))]
ratios_moment_pos_all_bridges = pd.DataFrame()
ratios_moment_neg_all_bridges = pd.DataFrame()
ratios_shear_all_bridges = pd.DataFrame()

truck_dataframes = {}

for dirpath, dirnames, filenames in os.walk(rootdir):
    print(os.path.relpath(dirpath))
    #print('#fn:', len(filenames))

    ratios_moment_pos_files = [file for file in filenames if "ratios_moment_pos_" in file and "Class_" in file]
    ratios_moment_neg_files = [file for file in filenames if "ratios_moment_neg_" in file and "Class_" in file]
    ratios_shear_files = [file for file in filenames if "ratios_shear_" in file and "Class_" in file]
    formatted_truck_files = [file for file in filenames if "_formatted.csv" in file and "Class_" in file]
    #time.sleep(2)
    #print(ratios_moment_pos_files)
    #print(formatted_truck_files)
    #Check for extreme_response files
    if ratios_moment_pos_files:
        
        #get span string. this is used in many file strings
        span_string = [file for file in filenames if file.startswith('IL_')][0][3:-4]
        span_name = [file for file in filenames if file.startswith('IL_')][0][3:-3]
        #print(span_name)
        span_name = span_name.replace('spans_', '; Lengths: ')
        #print(span_name)
        span_name = span_name.replace('_', 'ft, ')
        #print(span_name)
        span_name = span_name.replace('.', 'ft')
        #print(span_name)
        span_name = 'Spans: ' + span_name
        #print(span_name)
        
        #use influence line file to determine span since its name is predictable
        span_lengths = span_string.split('_')[1:]
        span_lengths = [float(length) for length in span_lengths]
        span_ratios = [ (length / span_lengths[0]) for length in span_lengths]
        
        #get support positions
        support_positions = span_lengths[:]
        support_positions.insert(0,0)
        support_positions = [sum(support_positions[0:index+1]) for index, pos in enumerate(support_positions)]
        support_ratios = [(pos / span_lengths[0]) for pos in support_positions]
        
        #get truck set files and truck set names
        truck_set_files = [file for file in filenames if file.endswith('_formatted.csv')]
        truck_set_files.remove(control_set_name + '_formatted.csv')
        truck_set_names = [string[:-14] for string in truck_set_files]
        
        #create blank dataframes to combine the different truck classes within
        ratios_moment_pos_df = pd.DataFrame()
        ratios_moment_neg_df = pd.DataFrame()
        ratios_shear_df = pd.DataFrame()

        left_neglect = []
        right_neglect = []
        for i in range(len(span_lengths)):
            for j in range(offset_neglect):
                left_neglect.append(support_positions[i + 1] - span_lengths[i] / 20 * (j + 1))
                right_neglect.append(support_positions[i] + span_lengths[i] / 20 * (j + 1))
        neglect = support_positions[:]
        neglect.extend(left_neglect)
        neglect.extend(right_neglect)

        for moment_pos_file, moment_neg_file, shear_file, truck_file in zip(ratios_moment_pos_files, ratios_moment_neg_files, ratios_shear_files,formatted_truck_files):
            
            class_name = truck_file[:-14]
            if class_name not in truck_dataframes:
                print(class_name)
                this_truck_set = pd.read_csv(dirpath + '/' + truck_file)
                gross_weight = this_truck_set.groupby('truck_index')['axle_weight'].sum()
                total_length = -this_truck_set.groupby('truck_index')['axle_rel_pos'].min()
                this_truck_set['axle_weight'] = this_truck_set['axle_weight'].astype(str) # takes a little time
                axle_weights = this_truck_set.groupby('truck_index')['axle_weight'].apply(lambda x: ','.join(x))
                this_truck_set['space'] = -this_truck_set.groupby('truck_index')['axle_rel_pos'].diff()
                spaces = this_truck_set[['truck_index','space']]
                spaces = spaces.dropna(axis=0,how='any')
                spaces['space'] = spaces['space'].apply(int).astype(str)# + 'ft'
                spaces = spaces.groupby('truck_index')['space'].apply(lambda x: ','.join(x))
                class_truck = spaces + 'ft - ' + axle_weights + 'k'
                class_truck = pd.DataFrame(class_truck,columns=['class_truck'])
                class_truck['gross_weight'] = gross_weight
                class_truck['total_length'] = total_length
                class_truck['class_truck_index'] = class_truck.index
                truck_dataframes[class_name] = class_truck

            #Positive Moment
            this_class = pd.read_csv(dirpath + '/' + moment_pos_file)
            #print(this_class)
            this_class.columns = [column.replace(class_name,'class').replace('_max','') for column in this_class.columns]
            drop_columns = [control_set_name+'_first_axle_pos',control_set_name+'_truck_direction','class_first_axle_pos','class_truck_direction']
            this_class = this_class.drop(drop_columns,axis=1)
            #print(class_name)
            idx = this_class[this_class['analysis_point'].isin(neglect)].index
            this_class = this_class.drop(idx)
            idx = this_class[this_class['ratio_moment_pos'] <= 1].index
            this_class = this_class.drop(idx)
            #print(this_class)
            this_class['truck_class'] = class_name
            this_class['bridge_span'] = span_string
            this_class = this_class.merge(truck_dataframes[class_name])
            ratios_moment_pos_df = ratios_moment_pos_df.append(this_class,ignore_index = True)
                       
            #Negative Moment
            this_class = pd.read_csv(dirpath + '/' + moment_neg_file)
            #print(this_class)
            this_class.columns = [column.replace(class_name,'class').replace('_max','') for column in this_class.columns]
            drop_columns = [control_set_name+'_first_axle_pos',control_set_name+'_truck_direction','class_first_axle_pos','class_truck_direction']
            this_class = this_class.drop(drop_columns,axis=1)
            #print(class_name)
            idx = this_class[this_class['ratio_moment_neg'] <= 1].index
            this_class = this_class.drop(idx)
            #print(this_class)
            this_class['truck_class'] = class_name
            this_class['bridge_span'] = span_string
            this_class = this_class.merge(truck_dataframes[class_name])
            ratios_moment_neg_df = ratios_moment_neg_df.append(this_class,ignore_index = True)

            #Shear
            this_class = pd.read_csv(dirpath + '/' + shear_file)
            #print(this_class)
            this_class.columns = [column.replace(class_name,'class').replace('_max','') for column in this_class.columns]
            drop_columns = [control_set_name+'_first_axle_pos',control_set_name+'_truck_direction','class_first_axle_pos','class_truck_direction']
            this_class = this_class.drop(drop_columns,axis=1)
            #print(class_name)
            idx = this_class[this_class['ratio_shear'] <= 1].index
            this_class = this_class.drop(idx)
            #print(this_class)
            this_class['truck_class'] = class_name
            this_class['bridge_span'] = span_string
            this_class = this_class.merge(truck_dataframes[class_name])
            ratios_shear_df = ratios_shear_df.append(this_class,ignore_index = True)
      
        #print(ratios_moment_pos_df)
        ratios_moment_pos_all_bridges = ratios_moment_pos_all_bridges.append(ratios_moment_pos_df, ignore_index = True)
        #print(ratios_moment_pos_all_bridges)
        
        ratios_moment_neg_all_bridges = ratios_moment_neg_all_bridges.append(ratios_moment_neg_df, ignore_index = True)
        #print(ratios_moment_neg_all_bridges)
        
        ratios_shear_all_bridges = ratios_shear_all_bridges.append(ratios_shear_df, ignore_index = True)
        #print(ratios_shear_all_bridges)
        
columns = list(ratios_moment_pos_all_bridges.columns)
#columns[1] = control_set_name + '_truck'
columns[columns.index(control_set_name + '_truck_index')] = control_set_name + '_truck'
ratios_moment_pos_all_bridges.columns = columns

columns = list(ratios_moment_neg_all_bridges.columns)
#columns[1] = control_set_name + '_truck'
columns[columns.index(control_set_name + '_truck_index')] = control_set_name + '_truck'
ratios_moment_neg_all_bridges.columns = columns

columns = list(ratios_shear_all_bridges.columns)
#columns[1] = control_set_name + '_truck'
columns[columns.index(control_set_name + '_truck_index')] = control_set_name + '_truck'
ratios_shear_all_bridges.columns = columns

if control_set_name == 'ArDOT':
    ratios_moment_pos_all_bridges[control_set_name + '_truck'].replace(1,'Code_4',inplace=True)
    ratios_moment_pos_all_bridges[control_set_name + '_truck'].replace(2,'Code_9',inplace=True)
    ratios_moment_pos_all_bridges[control_set_name + '_truck'].replace(3,'Code_5',inplace=True)
    
    ratios_moment_neg_all_bridges[control_set_name + '_truck'].replace(1,'Code_4',inplace=True)
    ratios_moment_neg_all_bridges[control_set_name + '_truck'].replace(2,'Code_9',inplace=True)
    ratios_moment_neg_all_bridges[control_set_name + '_truck'].replace(3,'Code_5',inplace=True)
    
    ratios_shear_all_bridges[control_set_name + '_truck'].replace(1,'Code_4',inplace=True)
    ratios_shear_all_bridges[control_set_name + '_truck'].replace(2,'Code_9',inplace=True)
    ratios_shear_all_bridges[control_set_name + '_truck'].replace(3,'Code_5',inplace=True)
    
elif control_set_name == 'AASHTO':
    ratios_moment_pos_all_bridges[control_set_name + '_truck'].replace(1,'Type_3',inplace=True)
    ratios_moment_pos_all_bridges[control_set_name + '_truck'].replace(2,'Type_3S2',inplace=True)
    ratios_moment_pos_all_bridges[control_set_name + '_truck'].replace(3,'Type_3-3',inplace=True)
    ratios_moment_pos_all_bridges[control_set_name + '_truck'].replace(4,'Type_SU4',inplace=True)
    ratios_moment_pos_all_bridges[control_set_name + '_truck'].replace(5,'Type_SU5',inplace=True)
    ratios_moment_pos_all_bridges[control_set_name + '_truck'].replace(6,'Type_SU6',inplace=True)
    ratios_moment_pos_all_bridges[control_set_name + '_truck'].replace(7,'Type_SU7',inplace=True)
    
    ratios_moment_neg_all_bridges[control_set_name + '_truck'].replace(1,'Type_3',inplace=True)
    ratios_moment_neg_all_bridges[control_set_name + '_truck'].replace(2,'Type_3S2',inplace=True)
    ratios_moment_neg_all_bridges[control_set_name + '_truck'].replace(3,'Type_3-3',inplace=True)
    ratios_moment_neg_all_bridges[control_set_name + '_truck'].replace(4,'Type_SU4',inplace=True)
    ratios_moment_neg_all_bridges[control_set_name + '_truck'].replace(5,'Type_SU5',inplace=True)
    ratios_moment_neg_all_bridges[control_set_name + '_truck'].replace(6,'Type_SU6',inplace=True)
    ratios_moment_neg_all_bridges[control_set_name + '_truck'].replace(7,'Type_SU7',inplace=True)
    
    ratios_shear_all_bridges[control_set_name + '_truck'].replace(1,'Type_3',inplace=True)
    ratios_shear_all_bridges[control_set_name + '_truck'].replace(2,'Type_3S2',inplace=True)
    ratios_shear_all_bridges[control_set_name + '_truck'].replace(3,'Type_3-3',inplace=True)
    ratios_shear_all_bridges[control_set_name + '_truck'].replace(4,'Type_SU4',inplace=True)
    ratios_shear_all_bridges[control_set_name + '_truck'].replace(5,'Type_SU5',inplace=True)
    ratios_shear_all_bridges[control_set_name + '_truck'].replace(6,'Type_SU6',inplace=True)
    ratios_shear_all_bridges[control_set_name + '_truck'].replace(7,'Type_SU7',inplace=True)
    
ratios_moment_pos_all_bridges.sort_values('ratio_moment_pos', ascending = False)[['ratio_moment_pos','bridge_span','truck_class','analysis_point','span_position','class_truck','gross_weight','total_length',control_set_name+'_truck','class_moment_pos',control_set_name+'_moment_pos']].to_csv('moment_positive_violation_vs_'+control_set_name+'_ordered_table.csv',index=False)
ratios_moment_neg_all_bridges.sort_values('ratio_moment_neg', ascending = False)[['ratio_moment_neg','bridge_span','truck_class','analysis_point','span_position','class_truck','gross_weight','total_length',control_set_name+'_truck','class_moment_neg',control_set_name+'_moment_neg']].to_csv('moment_negative_violation_vs_'+control_set_name+'_ordered_table.csv',index=False)
ratios_shear_all_bridges.sort_values('ratio_shear', ascending = False)[['ratio_shear','bridge_span','truck_class','analysis_point','span_position','class_truck','gross_weight','total_length',control_set_name+'_truck','class_shear',control_set_name+'_shear']].to_csv('shear_violation_vs_'+control_set_name+'_ordered_table.csv',index=False)
      
ws.Beep(1000,35)
ws.Beep(1500,35)
ws.Beep(1000,35)


