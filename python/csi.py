from tqdm import tqdm
import numpy as np
import pandas as pd
from pysolar.solar import *
import datetime as dt
from datetime import datetime
from math import cos
import os

file_list = os.listdir(r'C:\Users\for_someday\Desktop\divide18')
path = r'C:\Users\for_someday\Desktop\divide18'

np.random.seed(1)
permute_file_list = np.random.permutation(file_list)[100:125]

def csi(file_list, path):
    for csv_name in tqdm(file_list):
        file_name = csv_name[0:8]

        df = pd.read_csv(os.path.join(path, csv_name))
        

        col_list = list(df.columns)

        KST = dt.timezone(dt.timedelta(hours=9))
        for i in tqdm(range(len(df))):
            for j in range(2, len(col_list)):
                datehour = dt.timedelta(hours=int(col_list[j][:len(col_list[j])-2]))
                date_time = datetime.strptime(file_name, '%Y%m%d')
                date = date_time + datehour
                year = date.year
                month = date.month
                day = date.day
                hour = date.hour


                date = datetime(year, month, day, hour, 0, 0, tzinfo=KST)
                temp= get_altitude(df['lat'][i],df['lon'][i],date)   # 태양천정각
                sza = float(90) - temp
                
                new_value= sza*(math.pi/180)

                if temp < 0:
                    cosz = 0
                    df.iloc[i, j] = 0
                else:
                    cosz= math.cos(new_value)
                    df.iloc[i, j] = df.iloc[i, j] / 1.5 * cosz

        df.to_csv(f'C:/Users/for_someday/Desktop/18/{csv_name}')


csi(permute_file_list, path)