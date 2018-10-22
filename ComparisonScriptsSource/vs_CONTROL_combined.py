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
import sys

#control is the beginning of the filename for the control set of trucks
control_set_name = 'ArDOT'

max_plot_ratio = 2.0
offset_neglect = 3 #index of analysis points away from supports to neglect in moment graphs

start_time = timeit.default_timer()

rootdir = os.getcwd()
print(rootdir)


ap_ratios = [val / 20 for val in list(range(121))]
ratios_moment_pos_all_bridges = pd.DataFrame(index = ap_ratios)
ratios_moment_pos_all_bridges.index.name = 'span ↓ \ span position →'
ratios_moment_neg_all_bridges = pd.DataFrame(index = ap_ratios)
ratios_moment_neg_all_bridges.index.name = 'span ↓ \ span position →'
ratios_shear_all_bridges = pd.DataFrame(index = ap_ratios)
ratios_shear_all_bridges.index.name = 'span ↓ \ span position →'


for dirpath, dirnames, filenames in os.walk(rootdir):
    print(os.path.relpath(dirpath))
    #print('#fn:', len(filenames))

    ratios_moment_pos_files = [file for file in filenames if "ratios_moment_pos_" in file and "Class_" in file]
    ratios_moment_neg_files = [file for file in filenames if "ratios_moment_neg_" in file and "Class_" in file]
    ratios_shear_files = [file for file in filenames if "ratios_shear_" in file and "Class_" in file]

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
        ratios_moment_pos_df_single_bridge = pd.DataFrame()
        ratios_moment_neg_df_single_bridge = pd.DataFrame()
        ratios_shear_df_single_bridge = pd.DataFrame()
        
        for file in ratios_moment_pos_files:
            this_class = pd.read_csv(dirpath + '/' + file)
            #print(this_class)
            class_name = file.replace('ratios_moment_pos_','').replace('_vs_' + control_set_name + '_' + span_string + '.csv','')
            this_class.columns = [column.replace(class_name,'class') for column in this_class.columns]
            #print(class_name)
            
            this_class['truck_class'] = class_name
            ratios_moment_pos_df_single_bridge = ratios_moment_pos_df_single_bridge.append(this_class,ignore_index = True)
            #print(ratios_moment_pos_df_single_bridge)
        ratios_moment_pos_df_single_bridge = ratios_moment_pos_df_single_bridge.replace(np.inf, sys.maxsize)    #replace infinity with maxsize to allow idxmax in next line to work
        indices = ratios_moment_pos_df_single_bridge.groupby('analysis_point')['ratio_moment_pos_max'].idxmax()
        ratios_moment_pos_df_single_bridge = ratios_moment_pos_df_single_bridge.replace(sys.maxsize, np.inf)    #revert maxsize back to infinity
        ratios_moment_pos_df_single_bridge = ratios_moment_pos_df_single_bridge.loc[indices]
        ratios_moment_pos_df_single_bridge.to_csv(dirpath + '/' + 'ratios_moment_pos_All_vs_' + control_set_name + '_' + span_string + '.csv', index = False)

        
        for file in ratios_moment_neg_files:
            this_class = pd.read_csv(dirpath + '/' + file)
            #print(this_class)
            class_name = file.replace('ratios_moment_neg_','').replace('_vs_' + control_set_name + '_' + span_string + '.csv','')
            this_class.columns = [column.replace(class_name,'class') for column in this_class.columns]
            #print(class_name)
            
            this_class['truck_class'] = class_name
            ratios_moment_neg_df_single_bridge = ratios_moment_neg_df_single_bridge.append(this_class,ignore_index = True)
            #print(ratios_moment_neg_df_single_bridge)
            
        indices = ratios_moment_neg_df_single_bridge.groupby('analysis_point')['ratio_moment_neg_max'].idxmax()
        ratios_moment_neg_df_single_bridge = ratios_moment_neg_df_single_bridge.loc[indices]
        ratios_moment_neg_df_single_bridge.to_csv(dirpath + '/' + 'ratios_moment_neg_All_vs_' + control_set_name + '_' + span_string + '.csv', index = False)
        
        
        for file in ratios_shear_files:
            this_class = pd.read_csv(dirpath + '/' + file)
            #print(this_class)
            class_name = file.replace('ratios_shear_','').replace('_vs_' + control_set_name + '_' + span_string + '.csv','')
            this_class.columns = [column.replace(class_name,'class') for column in this_class.columns]
            #print(class_name)
            
            this_class['truck_class'] = class_name
            ratios_shear_df_single_bridge = ratios_shear_df_single_bridge.append(this_class,ignore_index = True)
            #print(ratios_shear_df_single_bridge)
            
        indices = ratios_shear_df_single_bridge.groupby('analysis_point')['ratio_shear_max'].idxmax()
        ratios_shear_df_single_bridge = ratios_shear_df_single_bridge.loc[indices]
        ratios_shear_df_single_bridge.to_csv(dirpath + '/' + 'ratios_shear_All_vs_' + control_set_name + '_' + span_string + '.csv', index = False)
        
        left_neglect = []
        right_neglect = []
        for i in range(len(span_lengths)):
            for j in range(offset_neglect):
                left_neglect.append(support_positions[i + 1] - span_lengths[i] / 20 * (j + 1))
                right_neglect.append(support_positions[i] + span_lengths[i] / 20 * (j + 1))
        neglect = support_positions[:]
        neglect.extend(left_neglect)
        neglect.extend(right_neglect)
        ratios_moment_pos_df_single_bridge = ratios_moment_pos_df_single_bridge.set_index('analysis_point')
        #print(ratios_moment_pos_df_single_bridge)
        ratios_moment_pos_df_single_bridge.loc[neglect, 'ratio_moment_pos_max'] = np.nan
        ratios_moment_pos_df_single_bridge.loc[neglect, 'truck_class'] = ''
        #print(ratios_moment_pos_df_single_bridge)

        x = ratios_moment_pos_df_single_bridge['span_position'].tolist()
        y = ratios_moment_pos_df_single_bridge['ratio_moment_pos_max'].tolist()
        label = ratios_moment_pos_df_single_bridge['truck_class'].tolist()
        label = [val.replace('Class_','') for val in label]
        
        supports = list(range(len(support_positions)))
        xmajor = FixedLocator(supports)
        xminor = FixedLocator([value for value in x if value not in supports])

        ymajor_ratio = np.arange(0, max_plot_ratio + 0.5, 0.5)
        yminor_ratio = np.arange(0, max_plot_ratio + 0.1, 0.1)
        ymajorindices_ratio = np.searchsorted(yminor_ratio, ymajor_ratio)
        yminor_ratio = np.delete(yminor_ratio, ymajorindices_ratio, None)
        yminor_ratio = FixedLocator(yminor_ratio)
        ymajor_ratio = FixedLocator(ymajor_ratio)        
        
        plt.figure(figsize = (16,9))
        plt.plot(x,y)
        plt.title('Positive Moment Ratios - All Classes vs ArDOT\n' + span_name)
        plt.xlabel('Span Position\nMajor Gridlines: Supports\nMinor Gridlines: 20ths Within Spans')
        plt.ylabel('Positive Moment Ratio')
        plt.ylim((0,max_plot_ratio))
        plt.xlim((0 - 0.05,x[-1] + 0.05))
        axes = plt.gca()
        above = True
        for a,b,c in zip(x,y,label):
            if not np.isnan(b):
                if above:
                    axes.text(a, 1.9, c, ha='center', va='bottom')
                    above = not above
                else:
                    axes.text(a, 1.9, c, ha = 'center', va = 'top')
                    above = not above
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
        for b in a:
        	#print(b.get_color())
        	b.set_color('0.70')
        	b.set_alpha(1)
        	#b.set_linewidth(10)
        	#print(b.get_color())
        	#print(b)
        a[2].set_color('0.25')
        a[2].set_linewidth(1.25)
        
        plt.savefig(dirpath + '/' + 'ratios_moment_pos_All_vs_' + control_set_name + '_' + span_string + '.png')
        plt.close()
        
        
        
        x = ratios_moment_neg_df_single_bridge['span_position'].tolist()
        y = ratios_moment_neg_df_single_bridge['ratio_moment_neg_max'].tolist()
        label = ratios_moment_neg_df_single_bridge['truck_class'].tolist()
        label = [val.replace('Class_','') for val in label]
        
        plt.figure(figsize = (16,9))
        plt.plot(x,y)
        plt.title('Negative Moment Ratios - All Classes vs ArDOT\n' + span_name)
        plt.xlabel('Span Position\nMajor Gridlines: Supports\nMinor Gridlines: 20ths Within Spans')
        plt.ylabel('Negative Moment Ratio')
        plt.ylim((0,max_plot_ratio))
        plt.xlim((0 - 0.05,x[-1] + 0.05))
        axes = plt.gca()
        above = True
        for a,b,c in zip(x,y,label):
            if not np.isnan(b):
                if above:
                    axes.text(a, 1.9, c, ha='center', va='bottom')
                    above = not above
                else:
                    axes.text(a, 1.9, c, ha = 'center', va = 'top')
                    above = not above
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
        for b in a:
        	#print(b.get_color())
        	b.set_color('0.70')
        	b.set_alpha(1)
        	#b.set_linewidth(10)
        	#print(b.get_color())
        	#print(b)
        a[2].set_color('0.25')
        a[2].set_linewidth(1.25)
        
        plt.savefig(dirpath + '/' + 'ratios_moment_neg_All_vs_' + control_set_name + '_' + span_string + '.png')
        plt.close()
        
        
        
        x = ratios_shear_df_single_bridge['span_position'].tolist()
        y = ratios_shear_df_single_bridge['ratio_shear_max'].tolist()
        label = ratios_shear_df_single_bridge['truck_class'].tolist()
        label = [val.replace('Class_','') for val in label]
        
        plt.figure(figsize = (16,9))
        plt.plot(x,y)
        plt.title('Shear Ratios - All Classes vs ArDOT\n' + span_name)
        plt.xlabel('Span Position\nMajor Gridlines: Supports\nMinor Gridlines: 20ths Within Spans')
        plt.ylabel('Shear Ratio')
        plt.ylim((0,max_plot_ratio))
        plt.xlim((0 - 0.05,x[-1] + 0.05))
        axes = plt.gca()
        above = True
        for a,b,c in zip(x,y,label):
            if not np.isnan(b):
                if above:
                    axes.text(a, 1.9, c, ha='center', va='bottom')
                    above = not above
                else:
                    axes.text(a, 1.9, c, ha = 'center', va = 'top')
                    above = not above
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
        for b in a:
        	#print(b.get_color())
        	b.set_color('0.70')
        	b.set_alpha(1)
        	#b.set_linewidth(10)
        	#print(b.get_color())
        	#print(b)
        a[2].set_color('0.25')
        a[2].set_linewidth(1.25)
        
        plt.savefig(dirpath + '/' + 'ratios_shear_All_vs_' + control_set_name + '_' + span_string + '.png')
        plt.close()
        
        #ratios_moment_pos_df_single_bridge.drop(neglect, inplace = True)
        #ratios_moment_pos_df_single_bridge.replace('','N/A', inplace = True)
        #ratios_moment_pos_df_single_bridge.set_index('span_position', inplace = True)
        index = ratios_moment_pos_df_single_bridge[ratios_moment_pos_df_single_bridge['ratio_moment_pos_max'] <= 1].index
        ratios_moment_pos_df_single_bridge['percent'] = ratios_moment_pos_df_single_bridge['ratio_moment_pos_max'] * 100 - 100
        ratios_moment_pos_df_single_bridge['combined'] = ratios_moment_pos_df_single_bridge['truck_class'] + ': ' + ratios_moment_pos_df_single_bridge['percent'].round(1).astype(str) + '%'
        ratios_moment_pos_df_single_bridge.loc[index, 'combined'] = '—'
        ratios_moment_pos_df_single_bridge.loc[neglect, 'combined'] = 'N/A'
        ratios_moment_pos_df_single_bridge.set_index('span_position', inplace = True)
        #ratios_moment_pos_all_bridges = ratios_moment_pos_all_bridges.append(ratios_moment_pos_df_single_bridge)
        ratios_moment_pos_all_bridges[span_string] = ratios_moment_pos_df_single_bridge['combined']
        ratios_moment_pos_all_bridges.fillna('').transpose().to_csv('All_vs_' + control_set_name + '_moment_positive_violation_table.csv')
        
        
        
        
        index = ratios_moment_neg_df_single_bridge[ratios_moment_neg_df_single_bridge['ratio_moment_neg_max'] <= 1].index
        ratios_moment_neg_df_single_bridge['percent'] = ratios_moment_neg_df_single_bridge['ratio_moment_neg_max'] * 100 - 100
        ratios_moment_neg_df_single_bridge['combined'] = ratios_moment_neg_df_single_bridge['truck_class'] + ': ' + ratios_moment_neg_df_single_bridge['percent'].round(1).astype(str) + '%'
        ratios_moment_neg_df_single_bridge.loc[index, 'combined'] = '—'
        ratios_moment_neg_df_single_bridge.set_index('span_position', inplace = True)
        ratios_moment_neg_all_bridges[span_string] = ratios_moment_neg_df_single_bridge['combined']
        ratios_moment_neg_all_bridges.fillna('').transpose().to_csv('All_vs_' + control_set_name + '_moment_negative_violation_table.csv')
        
        
        
        index = ratios_shear_df_single_bridge[ratios_shear_df_single_bridge['ratio_shear_max'] <= 1].index
        ratios_shear_df_single_bridge['percent'] = ratios_shear_df_single_bridge['ratio_shear_max'] * 100 - 100
        ratios_shear_df_single_bridge['combined'] = ratios_shear_df_single_bridge['truck_class'] + ': ' + ratios_shear_df_single_bridge['percent'].round(1).astype(str) + '%'
        ratios_shear_df_single_bridge.loc[index, 'combined'] = '—'
        ratios_shear_df_single_bridge.set_index('span_position', inplace = True)
        ratios_shear_all_bridges[span_string] = ratios_shear_df_single_bridge['combined']
        ratios_shear_all_bridges.fillna('').transpose().to_csv('All_vs_' + control_set_name + '_shear_violation_table.csv')
        
        
ws.Beep(1000,35)
ws.Beep(1500,35)
ws.Beep(1000,35)