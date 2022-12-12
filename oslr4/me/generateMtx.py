from random import randint

def GenerateMtx(order):
    mtx = [[]]
    for i in range(order):
        for __ in range(order):
            mtx[i].append(randint(0,15))
        mtx.append([])
    mtx.pop()
    return mtx

def CreateStrMtx(order):
    mtx = GenerateMtx(order)
    result = "["
    for array in mtx:
        result = result + "["
        for num in array:
            result = result + str(num)
            result = result +","
        result = result[:-1]
        result = result + "],"
    result = result[:-1]
    result = result + "]"
    return result

def MtxToStr(mtx):
    result = "["
    for array in mtx:
        result = result + "["
        for num in array:
            result = result + str(num)
            result = result +","
        result = result[:-1]
        result = result + "],"
    result = result[:-1]
    result = result + "]"
    return result
