# Dana jest tablica A[n] z długościami samochodów, które stoją w kolejce, żeby wjechać na prom. Prom ma dwa pasy (lewy i prawy), oba długości L. 
# Napisać program, który wyznacza, (które samochody powinny pojechać na który pas / ile samochodów może wjechać), żeby na promie zmieściło się jak najwięcej aut. 
# Auta mogą wjeżdżać w takiej kolejności, w jakiej są podane w tablicy A

# f - Czy da się pomieścić i aut na promie?
# f(i, l1, l2) = f(i-1, l1-A[i], l2) v f(i-1, l1, l2-A[i])
# argmax_{i=0,1,...,n-1}{ f(i, l, l) }