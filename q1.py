def conta_descendentes(v, T, L1):
    pass

def v_em_lista(v, lista):
    pass

def cria_l2(L1, T):
    # L2 tem que ser lista encadeada mas aq vo cria uma lista msm
    L2 = []
    
    # Array que armazena contagem de descendentes de cada vértice
    cont_desc = [0] * len[T]

    # Adiciona todos os elementos de L2 em L1
    for v in L1: # O(v^2)
        L2.append(v)

    # Se o vértice estiver na árvore e não estiver em L1
    for v in T and not v_em_lista(v, L1): # O(v^2)
        cont_desc = conta_descendentes(v, T, L1)
        if cont_desc % 2 == 1:
            L2.append(v)
    

