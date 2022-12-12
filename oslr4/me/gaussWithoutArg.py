import argparse


def printMTX(mtx, text, order=3)->None:
    print(text)
    for item in mtx:
        print(item)

def InverseOfMtx(mtx, order)->None:
    
    tmp = 0

    # Принтим матрицу
    #printMTX(mtx, "\tMatrix\t")

    # Добавляем справа нули
    for i in range(order):
        for ___ in range(order):
            mtx[i].append(0)
    # Создаем доп. единичную матрицу
    for i in range (order):
        for j in range(2*order):
            # Ставим единицы
                if(j == (i+order)): 
                    mtx[i][j] = 1
    
    for i in range(order - 1, 0, -1):
        if (mtx[i-1][0]<mtx[i][0]):
            # Свапаем строки матрицы
            temp = mtx[i]
            mtx[i] = mtx[i-1]
            mtx[i-1] = temp
    
    # Принтим матрицу которая получилась
    #printMTX(mtx, "\tAugmented Matrix\t", order=order*2)


    # Меняем строку на сумму самой себя и константы кратной другой строке матрицы
    for i in range(order):
        for j in range(order):
            if j!=i:
                temp = mtx[j][i]/mtx[i][i]
                for k in range(2*order):
                    mtx[j][k] -= mtx[i][k]*temp

    # Умножьте каждую строку на ненулевое целое число
    # Разделить элемент на диагональный элемент
    for i in range(order):
        temp = mtx[i][i]
        for j in range(2*order):
            mtx[i][j]=mtx[i][j]/temp
    
    # Убирем доп матрицу
    for i in range(order):
        for ___ in range(order):
            mtx[i].pop(0)
    
    for i in range(order):
        for j in range(order):
            mtx[i][j]=round(mtx[i][j],5)
    # Принтим ответ
    return mtx

def CheckChar(char):
    if char=='s' or char == 'e' or char==' ':
        return False
    else: 
        return True

def ParseArguments(mtx):
    mtx = mtx.replace(',',' ')
    mtx = mtx.replace('[', 's')
    mtx = mtx.replace(']', 'e')
    ar = []
    newMtx = []
    i = 0
    while(i!=len(mtx)):
        if mtx[i]=='s':
            i+=1
        elif mtx[i]=='e':
            newMtx.append(ar)
            ar = []
            i+=1
        elif mtx[i]==' ':
            i+=1
        else:
            tmp = mtx[i]
            while(CheckChar(mtx[i+1])):
                i+=1
                tmp+=mtx[i]
            ar.append(int(tmp))
            i+=1

    newMtx.pop()
    return newMtx