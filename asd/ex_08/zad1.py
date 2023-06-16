# Black Dorest to las rosnący na osi liczbowej. 
# Składa się z n drzew, znany jest zysk c_i, jaki można osiągnąć dla i-tego drzewa. 
# Uzyskać maksymalny zysk, jeśli nie można ściąć dwóch drzew z rzędu

# f(i) = max( f(i-1), f(i-2) + c_i )
#        c_0, i=0
#        max(c_0, c_1), i=1

T = []
cache = []
def wycinka(T, cache, i):
    if i == 0:
        return T[0]
    if i == 1:
        return max( T[0], T[1] )
    
    if cache[i] != -1:
        return cache[i]
    
    cache[i] = max(wycinka(T, cache, i-1), wycinka(T, cache, i-2), T[i])

    return cache[i]