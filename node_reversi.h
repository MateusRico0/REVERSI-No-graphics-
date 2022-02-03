struct element *Cratenode(){
    return NULL;
}

struct element *Newelement(int line, int column){
    struct element *new = (struct element*) malloc(sizeof(struct element));
    new->ply.line = line;
    new->ply.column = column;
    new->prev = NULL;
    new->next = NULL;
    return new;
}

struct element *Insert(struct element *list, int line, int column){
    struct element *new = Newelement(line,column);
    if (list == NULL){
        new->next = new;
        new->prev = new;
    }else{
        new->next = list;
        new->prev = list->prev;
        list->prev->next = new;
        list->prev = new;
    }
    list = new;
    return list;
}

struct play *Search(struct element *list, int line, int column){
    struct element *aux = list;
    if (list==NULL) return NULL;
    do{
        if (aux->ply.line == line && aux->ply.column == column) return &aux->ply;
        aux = aux->next;
    }while (aux != list);
    return NULL;
}

void Printlist(struct element *list){
    struct element *aux = list;
    if (aux==NULL) return;
    printf(">>>>>Possible Moves<<<<<\n");
    do{
        printf(" -Line %d column %d\n",aux->ply.line,aux->ply.column);
        aux = aux->next;
    }while(aux!=list);
}

struct element *Deletelist(struct element *list){
    struct element *aux;
    if (list==NULL) return NULL;
    list->prev->next = NULL;
    aux = list;
    while (aux!=NULL){
        list = list->next;
        free(aux);
        aux = list;
    }
    return NULL;
}
