# WIMFluence calculates shear and moment influence lines and the resulting shear and moment reactions based on weigh-in-motion data. Copyright © 2016-2018 Kenneth Pasley

# This file is part of WIMFluence.

# WIMFluence is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

# WIMFluence is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

# You should have received a copy of the GNU General Public License along with WIMFluence.  If not, see http://www.gnu.org/licenses.

import os
import winsound as ws
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import FixedLocator
import timeit

#control is the beginning of the filename for the control set of trucks
control_set_name = 'ArDOT'

max_plot_ratio = 2.0
offset_neglect = 3 #index of analysis points away from supports to neglect in moment graphs

start_time = timeit.default_timer()

rootdir = os.getcwd()
print(rootdir)

violation_list = open('%s\\violation_list_vs_%s.csv' %(rootdir,control_set_name), 'w')
violation_list.write('span_configuration,class,mom_pos_max_ratio,mom_pos_position,mom_pos_span_position,mom_neg_max_ratio,mom_neg_position,mom_neg_span_position,shear_max_ratio,shear_position,shear_span_position,violation')

for dirpath, dirnames, filenames in os.walk(rootdir):
    print(dirpath)
    #print('#fn:', len(filenames))

    extreme_response_files = [file for file in filenames if "_extreme_response_" in file]
    #Check for extreme_response files
    if extreme_response_files:
        #for file in filenames:
        #    if file.startswith('AASHTO') and file.endswith('.png') and not file.startswith('AASHTO_vs'):
        #        print(file)
        #        os.remove(dirpath + '\\' + file)
            
        
            
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
        support_positions = [ sum(support_positions[0:index+1]) for index, pos in enumerate(support_positions)]
        support_ratios = [ (pos / span_lengths[0]) for pos in support_positions]
        
        #get truck set files and truck set names
        truck_set_files = [file for file in filenames if file.endswith('_formatted.csv')]
        truck_set_files.remove(control_set_name + '_formatted.csv')
        truck_set_names = [string[:-14] for string in truck_set_files]
        
        #Check for more truck files than just the control
        #If any exist, proceed with analysis
        if truck_set_files:
            
            #control_set_trucks = pd.read_csv(dirpath + '/' + control_set_name + '_formatted.csv')
            
            control_set_response = pd.read_csv(dirpath + '/' + control_set_name + '_extreme_response_' + span_string + '.csv', header = 5)
            
            #find absolute max shear of left and right
            control_set_response['shear'] = control_set_response[['shear_left','shear_right']].abs().max(axis=1)
            
            #get max moments
            control_set_moment_pos_max_indices = control_set_response.groupby('analysis_point',sort=False)['moment'].idxmax()
            control_set_moment_pos_max = control_set_response.loc[control_set_moment_pos_max_indices].drop(['shear_left','shear_right','shear'], axis = 1)
            control_set_moment_pos_max.set_index('analysis_point', inplace = True)
            
            #get max shears
            control_set_shear_max_indices = control_set_response.groupby('analysis_point',sort=False)['shear'].idxmax()
            control_set_shear_max = control_set_response.loc[control_set_shear_max_indices].drop(['shear_left','shear_right','moment'], axis = 1)
            control_set_shear_max.set_index('analysis_point', inplace = True)
            
            #get min moments
            control_set_moment_neg_max_indices = control_set_response.groupby('analysis_point',sort=False)['moment'].idxmin()
            control_set_moment_neg_max = control_set_response.loc[control_set_moment_neg_max_indices].drop(['shear_left','shear_right','shear'], axis = 1)
            control_set_moment_neg_max.set_index('analysis_point', inplace = True)
            
            del(control_set_response)
            
            for this_set_name in truck_set_names:
                #get response 
                this_set_response = pd.read_csv(dirpath + '/' + this_set_name + '_extreme_response_' + span_string + '.csv', header = 5)
                
                #find absolute max shear of left and right
                this_set_response['shear'] = this_set_response[['shear_left','shear_right']].abs().max(axis=1)
                #get trucks (needed?)
                #this_set_trucks= pd.read_csv()
        
                #get max moments
                this_set_moment_pos_max_indices = this_set_response.groupby('analysis_point',sort=False)['moment'].idxmax()
                this_set_moment_pos_max = this_set_response.loc[this_set_moment_pos_max_indices].drop(['shear_left','shear_right','shear'], axis = 1)
                this_set_moment_pos_max.set_index('analysis_point', inplace = True)
                
                del(this_set_moment_pos_max_indices)

                #get max shears
                this_set_shear_max_indices = this_set_response.groupby('analysis_point',sort=False)['shear'].idxmax()
                this_set_shear_max = this_set_response.loc[this_set_shear_max_indices].drop(['shear_left','shear_right','moment'], axis = 1)
                this_set_shear_max.set_index('analysis_point', inplace = True)
                
                del(this_set_shear_max_indices)
                
                #get min moments
                this_set_moment_neg_max_indices = this_set_response.groupby('analysis_point',sort=False)['moment'].idxmin()
                this_set_moment_neg_max = this_set_response.loc[this_set_moment_neg_max_indices].drop(['shear_left','shear_right','shear'], axis = 1)
                this_set_moment_neg_max.set_index('analysis_point', inplace = True)
                
                del(this_set_moment_neg_max_indices)
                
                del(this_set_response)
                
                #new blank dataframes for comparison ratios
                comparison_ratios_moment_pos_max = pd.DataFrame()
                comparison_ratios_moment_neg_max = pd.DataFrame()
                comparison_ratios_shear_max = pd.DataFrame()
                
                #comparison analysis points
                comparison_ratios_moment_pos_max['analysis_point'] = control_set_moment_pos_max.index
                comparison_ratios_moment_neg_max['analysis_point'] = control_set_moment_pos_max.index
                comparison_ratios_shear_max['analysis_point'] = control_set_moment_pos_max.index
                
                #comparison analysis points as indices
                comparison_ratios_moment_pos_max.set_index('analysis_point', inplace = True)
                comparison_ratios_moment_neg_max.set_index('analysis_point', inplace = True)
                comparison_ratios_shear_max.set_index('analysis_point', inplace = True)
                
                ##find analysis point ratios
                #comparison_ratios_moment_pos_max['analysis_point_ratio'] = control_set_moment_pos_max.index / span_lengths[0]
                #comparison_ratios_moment_neg_max['analysis_point_ratio'] = control_set_moment_neg_max.index / span_lengths[0]
                #comparison_ratios_shear_max['analysis_point_ratio'] = control_set_shear_max.index / span_lengths[0]
                
                #find span positions
                analysis_points = list(comparison_ratios_moment_pos_max.index.unique())
                span_positions = [val / 20 for val in list(range(len(analysis_points)))]
                comparison_ratios_moment_pos_max['span_position'] = span_positions
                comparison_ratios_moment_neg_max['span_position'] = span_positions
                comparison_ratios_shear_max['span_position'] = span_positions


                #find ratios
                comparison_ratios_moment_pos_max['ratio_moment_pos_max'] = this_set_moment_pos_max['moment'] / control_set_moment_pos_max['moment']
                comparison_ratios_moment_neg_max['ratio_moment_neg_max'] = this_set_moment_neg_max['moment'] / control_set_moment_neg_max['moment']
                comparison_ratios_shear_max['ratio_shear_max'] = this_set_shear_max['shear'] / control_set_shear_max['shear']
                
                #replace NaN with 1
                comparison_ratios_moment_pos_max['ratio_moment_pos_max'].fillna(1, inplace = True)
                comparison_ratios_moment_neg_max['ratio_moment_neg_max'].fillna(1, inplace = True)
                comparison_ratios_shear_max['ratio_shear_max'].fillna(1, inplace = True)

                #get response values for this_set
                comparison_ratios_moment_pos_max[this_set_name + '_moment_pos_max'] = this_set_moment_pos_max['moment']
                comparison_ratios_moment_neg_max[this_set_name + '_moment_neg_max'] = this_set_moment_neg_max['moment']
                comparison_ratios_shear_max[this_set_name + '_shear_max'] = this_set_shear_max['shear']

                #get response values for the control_set
                comparison_ratios_moment_pos_max[control_set_name + '_moment_pos_max'] = control_set_moment_pos_max['moment']
                comparison_ratios_moment_neg_max[control_set_name + '_moment_neg_max'] = control_set_moment_neg_max['moment']
                comparison_ratios_shear_max[control_set_name + '_shear_max'] = control_set_shear_max['shear']

                #get truck indices for this_set
                comparison_ratios_moment_pos_max[this_set_name + '_truck_index'] = this_set_moment_pos_max['truck_index']
                comparison_ratios_moment_neg_max[this_set_name + '_truck_index'] = this_set_moment_neg_max['truck_index']
                comparison_ratios_shear_max[this_set_name + '_truck_index'] = this_set_shear_max['truck_index']
                
                #get truck directions for this_set
                comparison_ratios_moment_pos_max[this_set_name + '_truck_direction'] = this_set_moment_pos_max['truck_direction']
                comparison_ratios_moment_neg_max[this_set_name + '_truck_direction'] = this_set_moment_neg_max['truck_direction']
                comparison_ratios_shear_max[this_set_name + '_truck_direction'] = this_set_shear_max['truck_direction']
                
                #get first axle positions for this_set
                comparison_ratios_moment_pos_max[this_set_name + '_first_axle_pos'] = this_set_moment_pos_max['first_axle_pos']
                comparison_ratios_moment_neg_max[this_set_name + '_first_axle_pos'] = this_set_moment_neg_max['first_axle_pos']
                comparison_ratios_shear_max[this_set_name + '_first_axle_pos'] = this_set_shear_max['first_axle_pos']
                
                del(this_set_moment_pos_max)
                del(this_set_moment_neg_max)
                del(this_set_shear_max)
                
                #get truck indices for control_set
                comparison_ratios_moment_pos_max[control_set_name + '_truck_index'] = control_set_moment_pos_max['truck_index']
                comparison_ratios_moment_neg_max[control_set_name + '_truck_index'] = control_set_moment_neg_max['truck_index']
                comparison_ratios_shear_max[control_set_name + '_truck_index'] = control_set_shear_max['truck_index']
                
                #get truck directions for control_set
                comparison_ratios_moment_pos_max[control_set_name + '_truck_direction'] = control_set_moment_pos_max['truck_direction']
                comparison_ratios_moment_neg_max[control_set_name + '_truck_direction'] = control_set_moment_neg_max['truck_direction']
                comparison_ratios_shear_max[control_set_name + '_truck_direction'] = control_set_shear_max['truck_direction']
                
                #get first axle positions for control_set
                comparison_ratios_moment_pos_max[control_set_name + '_first_axle_pos'] = control_set_moment_pos_max['first_axle_pos']
                comparison_ratios_moment_neg_max[control_set_name + '_first_axle_pos'] = control_set_moment_neg_max['first_axle_pos']
                comparison_ratios_shear_max[control_set_name + '_first_axle_pos'] = control_set_shear_max['first_axle_pos']

                comparison_ratios_moment_pos_max.to_csv(dirpath + '/ratios_moment_pos_' + this_set_name + '_vs_' + control_set_name + '_' + span_string + '.csv')
                comparison_ratios_moment_neg_max.to_csv(dirpath + '/ratios_moment_neg_' + this_set_name + '_vs_' + control_set_name + '_' + span_string + '.csv')
                comparison_ratios_shear_max.to_csv(dirpath + '/ratios_shear_' + this_set_name + '_vs_' + control_set_name + '_' + span_string + '.csv')




                #Graphing
                supports = list(range(len(support_positions)))
                xmajor = FixedLocator(supports)
                xminor = FixedLocator([value for value in span_positions if value not in supports])
                
                ymajor_ratio = np.arange(0, max_plot_ratio + 0.5, 0.5)
                yminor_ratio = np.arange(0, max_plot_ratio + 0.1, 0.1)
                ymajorindices_ratio = np.searchsorted(yminor_ratio, ymajor_ratio)
                yminor_ratio = np.delete(yminor_ratio, ymajorindices_ratio, None)
                yminor_ratio = FixedLocator(yminor_ratio)
                ymajor_ratio = FixedLocator(ymajor_ratio)
                
                #Drop neglected moment APs
                left_neglect = []
                right_neglect = []
                for i in range(len(span_lengths)):
                    for j in range(offset_neglect):
                        left_neglect.append(support_positions[i + 1] - span_lengths[i] / 20 * (j + 1))
                        right_neglect.append(support_positions[i] + span_lengths[i] / 20 * (j + 1))
                neglect = support_positions[:]
                neglect.extend(left_neglect)
                neglect.extend(right_neglect)
                comparison_ratios_moment_pos_max.loc[neglect,'ratio_moment_pos_max'] = np.nan
                #comparison_ratios_moment_neg_max.loc[neglect,'ratio_moment_neg_max'] = np.nan
                
                
                violation_bool = False
                if (comparison_ratios_moment_pos_max['ratio_moment_pos_max'] > 1).any():
                    violation_bool = True
                elif (comparison_ratios_moment_neg_max['ratio_moment_neg_max'] > 1).any():
                    violation_bool = True
                elif (comparison_ratios_shear_max['ratio_shear_max'] > 1).any():
                    violation_bool = True
                                
                mom_pos_max = comparison_ratios_moment_pos_max['ratio_moment_pos_max'].max()
                mom_pos_max_ap = comparison_ratios_moment_pos_max[comparison_ratios_moment_pos_max['ratio_moment_pos_max'] == mom_pos_max].index.min()
                mom_pos_max_ap_ratio = comparison_ratios_moment_pos_max[comparison_ratios_moment_pos_max['ratio_moment_pos_max'] == mom_pos_max]['span_position'].min()                
                
                mom_neg_max = comparison_ratios_moment_neg_max['ratio_moment_neg_max'].max()
                mom_neg_max_ap = comparison_ratios_moment_neg_max[comparison_ratios_moment_neg_max['ratio_moment_neg_max'] == mom_neg_max].index.min()
                mom_neg_max_ap_ratio = comparison_ratios_moment_neg_max[comparison_ratios_moment_neg_max['ratio_moment_neg_max'] == mom_neg_max]['span_position'].min()                
                
                shear_max = comparison_ratios_shear_max['ratio_shear_max'].max()
                shear_max_ap = comparison_ratios_shear_max[comparison_ratios_shear_max['ratio_shear_max'] == shear_max].index.min()
                shear_max_ap_ratio = comparison_ratios_shear_max[comparison_ratios_shear_max['ratio_shear_max'] == shear_max]['span_position'].min()                


                
                violation_list.write('\n%s,%s,%.2f,%d,%.2f,%.2f,%d,%.2f,%.2f,%d,%.2f,%s' %(span_string,this_set_name,mom_pos_max,mom_pos_max_ap,mom_pos_max_ap_ratio,mom_neg_max,mom_neg_max_ap,mom_neg_max_ap_ratio,shear_max,shear_max_ap,shear_max_ap_ratio,violation_bool))

                
                
                plt.figure(figsize = (16,9))
                
                plt.plot(comparison_ratios_moment_pos_max['span_position'], comparison_ratios_moment_pos_max['ratio_moment_pos_max'], label = 'Positive Moment Ratio')
                plt.plot(comparison_ratios_moment_neg_max['span_position'], comparison_ratios_moment_neg_max['ratio_moment_neg_max'], label = 'Negative Moment Ratio')
                plt.plot(comparison_ratios_shear_max['span_position'], comparison_ratios_shear_max['ratio_shear_max'], label = 'Shear Ratio')



                plt.title(this_set_name.replace('_',' ') + ' vs. ' + control_set_name.replace('_',' ') + ' Ratio\n' + span_name)
                plt.ylabel('Ratio')
                plt.xlabel('Span Position\nMajor Gridlines: Supports\nMinor Gridlines: 20ths Within Spans')
                plt.legend(loc='lower center', ncol = 7)
                plt.ylim((0,max_plot_ratio))
                axes = plt.gca()
                axes.xaxis.set_major_locator(xmajor)
                axes.xaxis.set_minor_locator(xminor)
                axes.yaxis.set_minor_locator(yminor_ratio)
                axes.yaxis.set_major_locator(ymajor_ratio)
                plt.grid(which='major', linestyle='-', color = 'k')
                plt.grid(which='minor', linestyle='-', color = '0.85')
                axes.spines['top'].set_linewidth(2)
                axes.spines['right'].set_linewidth(2)
                axes.spines['bottom'].set_linewidth(2)
                axes.spines['left'].set_linewidth(2)
                a = axes.get_ygridlines()
                #print(len(a))
                for b in a:
                	#print(b.get_color())
                	b.set_color('0.70')
                	b.set_alpha(1)
                	#b.set_linewidth(10)
                	#print(b.get_color())
                	#print(b)
                a[2].set_color('0.25')
                a[2].set_linewidth(1.25)
                axes.set_xticklabels([str(s) + '\n' + str(int(p)) + ' ft' for s,p in zip(supports,support_positions)])
        	
                #input('asdf')
        	
                #plt.show()
                plt.savefig(dirpath + '/' + this_set_name + '_vs_' + control_set_name + '_' + span_string)
                #plt.savefig(imagename, bbox_inches='tight')
                plt.close()
             
violation_list.close()

stop_time = timeit.default_timer()

print(stop_time - start_time)

ws.Beep(1000,35)
ws.Beep(1500,35)
ws.Beep(1000,35)