import matplotlib.pyplot as plt
from math import log10
from numpy import arange, linspace, mean
from scipy.stats import expon, zscore, norm
import matplotlib.font_manager as fm
import matplotlib

# 시뮬레이션 함수 생성
def test (times):
    
    t = times
    l = 10
    loc = 0

    m = []

    for i in arange(t):
       m.append(mean(expon(loc, l).rvs(size=30)))
    
    z = zscore(m)
    
    b = int(6 * log10(t))
    font_name = fm.FontProperties(fname="NanumGothicCoding.ttf", size=12).get_name()
    matplotlib.rc('font', family=font_name)
    fig = plt.figure(figsize=(12, 6))

    ax1 = fig.add_subplot(1, 2, 1)
    ax1 = plt.hist(m, bins=b, facecolor='wheat')
    ax1 = plt.xlabel('m')
    ax1 = plt.ylabel('frequency')
    ax1 = plt.title('n이 10000일 때 랜덤 히스토그램')
    ax1 = plt.grid()

    x = linspace(-3, 3, 101)

    ax2 = fig.add_subplot(1, 2, 2)
    ax2 = plt.hist(z, bins=b, density=True, facecolor='skyblue')
    ax2 = plt.plot(x, norm(0, 1).pdf(x), 'r--')
    ax2 = plt.xlabel('z')
    ax2 = plt.ylabel('density')
    ax2 = plt.title('n이 10000일 때 랜덤 히스토그램')
    ax2 = plt.grid()

    plt.show()

test(10000)
