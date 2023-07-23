def mergesort(tab, l, p):
    if l == p:
        return
    mid = (l + p) // 2
    mergesort(tab, l, mid)
    mergesort(tab, mid + 1, p)
    
    merge(tab, l, p, mid)
    return tab


def merge(tab, l, p, mid):
    #left subarray
    l_sub = tab[l:mid + 1]
    # right subarray
    r_sub = tab[mid + 1:p + 1]

    l = 0
    p = 0
    s = l

    while l < len(l_sub) and p < len(r_sub):
        if l_sub[l] < r_sub[p]:
            tab[s] = l_sub[l]
            l += 1
        else:
            tab[s] = r_sub[p]
            p += 1
        s += 1

    while l < len(l_sub):
        tab[s] = l_sub[l]
        l += 1
        s += 1

    while p < len(r_sub):
        tab[s] = r_sub[p]
        p += 1
        s += 1
