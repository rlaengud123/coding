# To add a new cell, type '# %%'
# To add a new markdown cell, type '# %% [markdown]'
# %%
import pandas as pd
import tqdm
import os
import numpy as np

# from google.colab import drive
# drive.mount('/content/gdrive/')


# %%
# data = pd.read_csv("/content/gdrive/My Drive/Colab Notebooks/hwayo.csv", encoding='utf-8')
data = pd.read_csv("hwayo.csv", encoding='utf-8-sig')


data = data.drop(data.columns[[0,1,4,5,6,7,8,9,10,12,13,14,16,18,19]], axis='columns')

data = data.drop([0,1],0)

data.columns=['product','volume','quantity','price','locate'] #거래일, 거래처, 물품, 수량, 가격, 출고지명

data = data.reset_index()

data = data.drop(data.columns[[0]], axis='columns')

data = data.drop(len(data)-1)

data = data.fillna(value='0')

data = data[data.locate != '기타']

data = data.reset_index()

data = data.drop(data.columns[[0]], axis='columns')

data['volume'] = pd.to_numeric(data['volume'])



for i in tqdm.trange(len(data)):
  data['price'][i] = int(data['price'][i].replace(",",""))
  data['quantity'][i] = int(data['quantity'][i].replace(",",""))


# 직영점 기타, 기타는 제거


# %%
# table = pd.read_csv("/content/gdrive/My Drive/Colab Notebooks/table.csv", encoding='utf-8')
table = pd.read_csv("table.csv", encoding='utf-8-sig')
table.rename(columns = {'Unnamed: 0' : 'alcohol', 'Unnamed: 1' : 'volume'}, inplace = True)
# product_list = pd.read_csv("/content/gdrive/My Drive/Colab Notebooks/product_list.csv", encoding='utf-8')
product_list = pd.read_csv("product_list.csv", encoding='utf-8-sig')


# %%
for i in range(0,len(data)):
  if data['locate'][i] == '직영점':
    locate1 = ' 기타1 '
    locate2 = ' 기타2 '
  elif data['locate'][i] == '업소':
    locate1 = ' 업소1 '
    locate2 = ' 업소2 '
  elif data['locate'][i] == '계열사':
    locate1 = ' 기타1 '
    locate2 = ' 기타1 '
  elif data['locate'][i] == '소매점':
    locate1 = ' 소매1 '
    locate2 = ' 소매2 '
  elif data['locate'][i] == '면세':
    locate1 = ' 면세1 '
    locate2 = ' 면세2 '
  elif data['locate'][i] == '수출':
    locate1 = ' 수출1 '
    locate2 = ' 수출2 '
  elif data['locate'][i] == '영남팀':
    locate1 = ' 영남팀1 '
    locate2 = ' 영남팀2 '
  elif data['locate'][i] == '유통':
    locate1 = ' 유통1 '
    locate2 = ' 유통2 '
  volume = int(data['volume'][i])
  price = int(data['price'][i])
  quantity = int(data['quantity'][i])
  for j in range(0,len(product_list)):
    if data['product'][i] == product_list['product'][j]:
      alcohol = product_list['alcohol'][j]
      for k in range(0, len(table)):
        if alcohol == 1 or alcohol == 2 or alcohol == 3 or alcohol == 4 or alcohol == 5:
          if table['alcohol'][k] == alcohol:
            table[locate1][k] += quantity
            table[locate2][k] += price
        else:
          if table['alcohol'][k] == alcohol:
            if str(volume) == table['volume'][k]:
              table[locate1][k] += quantity
              table[locate2][k] += price
        



      


# %%
# table.to_csv("/content/gdrive/My Drive/Colab Notebooks/result.csv", header=True, index=False, encoding='cp949')
table.to_csv("result.csv", header=True, index=False, encoding='utf-8-sig')


# %%
table


# %%

# 

# 

# 


# data['buyer_code'] = pd.to_numeric(data['buyer_code'])

# code_index = data[data['buyer_code'] > 90000].index

# data = data.drop(index = code_index)

# data = data.reset_index()

# data = data.drop(data.columns[[0]], axis='columns')

# for _ in tqdm.trange(len(data['date'])):
#     data['date'][_] = str(data['date'][_].replace("-",""))
#     data['quantity'][_] = int(data['quantity'][_].replace(",",""))
#     data['price'][_] = int(data['price'][_].replace(",",""))
#     data['buyer_code'][_] = int(data['buyer_code'][_])


# original2 = data #전처리 후 저장

# original2.to_csv("/content/gdrive/My Drive/Colab Notebooks/delete/pre-data.csv", header=True, index=False, encoding='cp949')

