# To add a new cell, type '# %%'
# To add a new markdown cell, type '# %% [markdown]'
# %%
import pandas as pd
import tqdm
import os
import numpy as np



# font_location = 'C:\\Windows\Fonts\나눔고딕코딩.ttf'
# font_name = fm.FontProperties(fname=font_location).get_name()


# %%
data = pd.read_csv("./data/hwayo.csv", encoding='utf-8-sig')

data = data.drop(data.columns[[1,2,5,6,8,9,11,12,14,15,16,17,18,19,20,21,22,23]], axis='columns')

data = data.drop([0,1],0)

data.columns=['date','buyer_code','buyer','product','quantity','price','locate'] #거래일, 거래처, 물품, 수량, 가격, 출고지명

data = data.dropna(axis = 0)

data = data.reset_index()

data = data.drop(data.columns[[0]], axis='columns')


data['buyer_code'] = pd.to_numeric(data['buyer_code'])

code_index = data[data['buyer_code'] > 90000].index

data = data.drop(index = code_index)

data = data.reset_index()

data = data.drop(data.columns[[0]], axis='columns')

for _ in tqdm.trange(len(data['date'])):
    data['date'][_] = str(data['date'][_].replace("-",""))
    data['quantity'][_] = int(data['quantity'][_].replace(",",""))
    data['price'][_] = int(data['price'][_].replace(",",""))
    data['buyer_code'][_] = int(data['buyer_code'][_])


original2 = data #전처리 후 저장

original2.to_csv("./delete/pre-data.csv", header=True, index=False, encoding='utf-8-sig')



data_buyer = pd.read_csv("./delete/pre-data.csv", encoding='utf-8-sig')

data_buyer = data_buyer.drop(data_buyer.columns[[2,6]], axis='columns')

per_buyer = data_buyer.groupby(['date', 'product','buyer_code']).sum()

per_buyer.to_csv("./delete/per_buyer.csv", header=True, index=True, encoding='utf-8-sig')



table = pd.read_csv("./dont_delete/table.csv", encoding='utf-8', error_bad_lines=False)
table2 = pd.read_csv("./dont_delete/table2.csv", encoding='utf-8', error_bad_lines=False)


data_buyer = pd.read_csv("./delete/per_buyer.csv", encoding='utf-8-sig')


for i in tqdm.trange(len(data_buyer)):
  for j in range(0,len(table)):
    if data_buyer['buyer_code'][i] == table['거래처코드'][j]:
      if data_buyer['product'][i] == table['품목'][j]:
        table['개수'][j] = table['개수'][j] + data_buyer['quantity'][i]
        table['매출금액'][j] = table['매출금액'][j] + data_buyer['price'][i]
    if len(table2) > j:
      if data_buyer['buyer_code'][i] == table2['거래처코드'][j]:
        table2['매출금액'][j] = table2['매출금액'][j] + data_buyer['price'][i]

sum_qun_1 = 0
sum_qun_2 = 0
sum_qun_3 = 0

sum_pri_1 = 0
sum_pri_2 = 0
sum_pri_3 = 0

for i in tqdm.trange(len(table)):
  if table['품목'][i] != '합 계':
    sum_qun_1 += table['개수'][i]
    sum_qun_2 += table['개수'][i]
    sum_qun_3 += table['개수'][i]
    sum_pri_1 += table['매출금액'][i]
    sum_pri_2 += table['매출금액'][i]
    sum_pri_3 += table['매출금액'][i]
  else:
    table['개수'][i] = sum_qun_1
    sum_qun_1 = 0
    table['매출금액'][i] = sum_pri_1
    sum_pri_1 = 0
  if table['품목'][i] == '총 합계':
    table['개수'][i] = sum_qun_2
    sum_qun_2 = 0
    table['매출금액'][i] = sum_pri_2
    sum_pri_2 = 0
  if table['구분1'][i] == '총      합      계':
    table['개수'][i] = sum_qun_3
    table['매출금액'][i] = sum_pri_3
    sum_qun_3 = 0
    sum_pri_3 = 0

table['개수'][len(table)-1] = sum_qun_3
table['매출금액'][len(table)-1] = sum_pri_3


sum_pri = 0

for i in tqdm.trange(len(table2)):
    sum_pri += table2['매출금액'][i]

table2['매출금액'][len(table2)-1] = sum_pri

for i in tqdm.trange(len(table)):
  if table['개수'][i] == 0:
    table['개수'][i] = '-'
    table['매출금액'][i] = '-'

for i in tqdm.trange(len(table2)):
  if table2['매출금액'][i] == 0:
    table2['매출금액'][i] = '-'  

table.to_csv("./result/result.csv", header=True, index=False, encoding='utf-8-sig')
table2.to_csv("./result/result2.csv", header=True, index=False, encoding='utf-8-sig')
