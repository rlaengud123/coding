#%%
import pandas as pd
import tqdm
import os
import matplotlib
import matplotlib.pyplot as plt
import datetime
import matplotlib.dates as mdates
from matplotlib.dates import DateFormatter
import matplotlib.font_manager as fm
import numpy as np

#%%
while(True):
    path = './dont_delete'

    print("파일을 선택하여 주세요.\n")
    print("1 : table.csv (마트, 백화점, 편의점)\n")
    print("2 : table2.csv (소매점)\n")
    print("n : 저장 및 종료하기\n")
    table_num = str(input())
    print("\n")
    if table_num == '1':
        table_name = "table.csv"
        print(f"{table_name}이 선택되었습니다.\n")
    elif table_num == '2':
        table_name = "table2.csv"
        print(f"{table_name}이 선택되었습니다.\n")
    elif table_num == 'n':
        df.to_csv(file_path, index=False)
        break
    else:
        print("잘못입력하셧습니다.\n")

    file_path = path + "/" + table_name
    df = pd.read_csv(file_path)
#%%
    while(True):
        print("옵션을 선택하여 주세요.\n")
        print("1 : 추가\n")
        print("2 : 삭제\n")
        print("3 : 검색하기\n")
        print("n : 뒤로가기\n")

        option_num = str(input())
        print("\n")

        if option_num == '1':
            print("추가하기 입니다.\n")
            print("구분1을 입력하여 주세요. 뒤로 가시려면 n을 입력해주세요.\n")
            option_num_1_1 = str(input())
            if option_num_1_1 == 'n':
                    break
            if table_num == '1':
                print("구분2을 입력하여 주세요. 뒤로 가시려면 n을 입력해주세요.\n")
                option_num_1_2 = str(input())
                if option_num_1_2 == 'n':
                        break
            print("거래처코드을 입력하여 주세요. 뒤로 가시려면 n을 입력해주세요.\n")
            option_num_1_3 = str(input())
            if option_num_1_3 == 'n':
                    break
            print("도매장명을 입력하여 주세요. 뒤로 가시려면 n을 입력해주세요.\n")
            option_num_1_4 = str(input())
            if option_num_1_4 == 'n':
                    break
            if table_num == '1':
                print("품목을 입력하여 주세요. 뒤로 가시려면 n을 입력해주세요.\n")
                option_num_1_5 = str(input())
                if option_num_1_5 == 'n':
                        break
            if table_num == '1':
                
            elif table_num == '2':
                

        elif option_num == '2':
            print("삭제하기 입니다.\n")
            if table_num == '1':
                print(df['구분1'])
                print("삭제할 구분1을 입력하여 주세요. 뒤로 가시려면 n을 입력해주세요.\n")
                option_num_2_1 = str(input())
                if option_num_2_1 == 'n':
                        break
                if option_num_2_1 not in list(set(df['구분1'])):
                    print('존재하지 않는 단어입니다. 다시 확인 해주세요.\n')
                else:
                    df1 = df[df['구분1'] == option_num_2_1]
                    print(df[df['구분1'] == option_num_2_1])
                    print("삭제할 구분2을 입력하여 주세요. 뒤로 가시려면 n을 입력해주세요.\n")
                    option_num_2_2 = str(input())
                    if option_num_2_2 == 'n':
                        break
                    if option_num_2_2 not in list(set(df1['구분2'])):
                        print('존재하지 않는 단어입니다. 다시 확인 해주세요.\n')
                    else:
                        df2 = df1[df1['구분2'] == option_num_2_2]
                        print(df2)
                        print("거래처코드를 입력하여 주세요. 뒤로 가시려면 n을 입력해주세요.")
                        onption_num2_3 = str(input())
                        if onption_num2_3 == 'n':
                            break
                        if int(onption_num2_3) not in list(set(df2['거래처코드'])):
                            print('존재하지 않는 단어입니다. 다시 확인 해주세요.\n')
                        else:
                            df3 = df2[df2['거래처코드'] == int(onption_num2_3)]
                            print(df3)
                            print(
                                """특정 제품을 지우시려면 i을 입력해주세요.\n모두 삭제하려면 a를 입력해주세요.\n뒤로 돌아가려면 n을 입력해주세요.\n""")
                            option_num_2_2_1 = str(input())
                            if option_num_2_2_1 == 'n':
                                break
                            elif option_num_2_2_1 == 'i':
                                print(df3)
                                print("어떤 제품을 지우실건가요? 뒤로 가시려면 n을 입력해주세요.\n")
                                option_num_2_2_2 = str(input())
                                if option_num_2_2_2 == 'n':
                                    break
                                if option_num_2_2_2 not in list(set(df3['품목'])):
                                    print("제품을 찾을 수 없습니다.")
                                else:
                                    df4 = df3[df3['품목'] == option_num_2_2_2]
                                    print(df4)
                                    print("지워도 될까요?\n")
                                    print("네 : y\n")
                                    print("아니오 : n\n")
                                    answer = str(input())
                                    if answer == 'y':
                                        df = df.drop(index=df4.index)
                                        df = df.reset_index()
                                        df = df.drop(
                                            df.columns[[0]], axis='columns')
                                        print("삭제되었습니다.\n")
                                        break
                                    elif answer == 'n':
                                        print("취소되었습니다.\n")
                                        break
                                    else:
                                        print("잘못된 입력입니다.")
                            elif option_num_2_2_1 == 'a':
                                print("전체 지워도 될까요\n")
                                print("네 : y\n")
                                print("아니오 : n\n")
                                answer = str(input())
                                if answer == 'y':
                                    df = df.drop(index=df3.index)
                                    df = df.reset_index()
                                    df = df.drop(
                                        df.columns[[0]], axis='columns')
                                    print(df)
                                    print("삭제되었습니다.\n")
                                    break
                                elif answer == 'n':
                                    print("취소되었습니다.\n")
                                    break
                                else:
                                    print("잘못된 입력입니다.")
                            else:
                                print("잘못된 입력입니다.")
            elif table_num == '2':
                print(df['구분1'])
                print("삭제할 구분1을 입력하여 주세요. 뒤로 가시려면 n을 입력해주세요.\n")
                option_num_2_1 = str(input())
                if option_num_2_1 == 'n':
                        break
                if option_num_2_1 not in list(set(df['구분1'])):
                    print('존재하지 않는 단어입니다. 다시 확인 해주세요.\n')
                else:
                    df1 = df[df['구분1'] == option_num_2_1]
                    print(df[df['구분1'] == option_num_2_1])
                    print("삭제할 거래처코드를 입력하여 주세요. 뒤로 가시려면 n을 입력해주세요.\n")
                    option_num_2_2 = str(input())
                    if option_num_2_2 == 'n':
                        break
                    if int(option_num_2_2) not in list(set(df1['거래처코드'])):
                        print('존재하지 않는 단어입니다. 다시 확인 해주세요.\n')
                    else:
                        df2 = df1[df1['거래처코드'] == int(option_num_2_2)]
                        print(df2)
                        print("지워도 될까요?\n")
                        print("네 : y\n")
                        print("아니오 : n\n")
                        answer = str(input())
                        if answer == 'y':
                            df = df.drop(index=df2.index)
                            df = df.reset_index()
                            df = df.drop(
                                df.columns[[0]], axis='columns')
                            print("삭제되었습니다.\n")
                            break
                        elif answer == 'n':
                            print("취소되었습니다.\n")
                            break
                        else:
                            print("잘못된 입력입니다.")
        elif option_num == '3':
            print("검색하기 입니다.\n")
            print("어떤 것을 검색하시겠습니까?\n")
            print("1 : 구분1\n")
            print("2 : 구분2\n")
            print("3 : 도매장명\n")
            print("n : 뒤로가기\n")
            option_num_3 = str(input())
            if option_num_3 == '1':
                print("검색할 단어(구분1)을 입력해 주세요.\n")
                option_num_3_1 = str(input())
                if option_num_3_1 not in list(set(df['구분1'])):
                    print('존재하지 않는 단어입니다. 다시 확인 해주세요.\n')
                else:
                    print(df[df['구분1'] == option_num_3_1])
            elif option_num_3 == '2':
                print("검색할 단어(구분2)을 입력해 주세요.\n")
                option_num_3_2 = str(input())
                if option_num_3_2 not in list(set(df['구분2'])):
                    print('존재하지 않는 단어입니다. 다시 확인 해주세요.\n')
                else:
                    print(df[df['구분2'] == option_num_3_2])
            elif option_num_3 == '3':
                print("검색할 단어(도매장명)을 입력해 주세요.\n")
                option_num_3_3 = str(input())
                if option_num_3_3 not in list(set(df['도매장명'])):
                    print('존재하지 않는 단어입니다. 다시 확인 해주세요.\n')
                else:
                    print(df[df['도매장명'] == option_num_3_3])
            elif option_num_3 == 'n':
                break
            else:
                print("잘못입력하셨습니다.\n")
        elif option_num == 'n':
            break
        else:
            print("잘못입력하였습니다.\n")


# %%
