# Mapa: wierzchołki to miasta, krawędzie to drogi, wagi to to długości dróg
# Dwie osoby jadą jednym samochodem, co miasto zmieniają miejsce kierowcy
# Jedna osoba ma przejechać jak najmniej kilometrów, u drugiej nie ma założeń

# Pomysł 1: "rozmnożyć" weichołki i krawędzie tak, aby pierwsza osoba jeździła 
# normalnymi drogami, a druga z zerowymi wagami

# Pomysł 2: Użyć algorytmu Dijsktry, ale zapisywać też kto jest kierowcą