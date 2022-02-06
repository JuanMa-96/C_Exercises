#include "lista.h"
#include "funciones.h"

#define MINIMO(X,Y) ( (X) < (Y) ? (X) : (Y) )

void crearLista(tLista *p)
{
    *p = NULL;
}

int listaVacia(const tLista *p)
{
    return *p == NULL;
}

int listaLlena(const tLista *p, unsigned cantBytes)
{
    tNodo *aux = (tNodo*)malloc(sizeof(tNodo));
    void *info = malloc(cantBytes);

    free(aux);
    free(info);

    return aux == NULL || info == NULL;
}

void vaciarLista(tLista *p)
{
    while(*p)
    {
        tNodo *aux = *p;

        *p = aux->sig;
        free(aux->info);
        free(aux);
    }
}

int ponerAlComienzo(tLista *p, const void *d, unsigned cantBytes)
{
    tNodo *nue;

    if( (nue = (tNodo*)malloc(sizeof(tNodo))) == NULL ||
            (nue->info = malloc(cantBytes)) == NULL )
    {
        free(nue);
        return 0;
    }

    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;
    nue->sig = *p;
    *p = nue;

    return 1;
}
int sacarPrimeroLista(tLista *p, void *d, unsigned cantBytes)
{
    tNodo *aux = *p;

    if(aux == NULL)
        return 0;

    *p = aux->sig;
    memcpy(d, aux->info, MINIMO(cantBytes, aux->tamInfo));
    free(aux->info);
    free(aux);

    return 1;
}

int verPrimeroLista(const tLista *p, void *d, unsigned cantBytes)
{
    if(*p == NULL)
        return 0;

    memcpy(d, (*p)->info, MINIMO(cantBytes, (*p)->tamInfo));

    return 1;
}

int ponerAlFinal(tLista *p, void *d, unsigned cantBytes)
{
    tNodo *nue;

    while(*p)
        p = &(*p)->sig;

    if( (nue = (tNodo*)malloc(sizeof(tNodo))) == NULL ||
            (nue->info = malloc(cantBytes)) == NULL)
    {
        free(nue);
        return 0;
    }

    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;
    nue->sig = NULL;
    *p = nue;

    return 1;
}

int sacarUltimoLista(tLista *p, void *d, unsigned cantBytes)
{
    tNodo *aux = *p;

    if(*p == NULL)
        return 0;

    while(*p)
        p = &(*p)->sig;

    memcpy(d, aux->info, MINIMO(cantBytes, aux->tamInfo));
    free(aux->info);
    free(aux);

    *p = NULL;

    return 1;
}

int verUltimoLista(const tLista *p, void *d, unsigned cantBytes)
{
    if(*p == NULL)
        return 0;

    while((*p)->sig)
        p = &(*p)->sig;

    memcpy(d, (*p)->info, MINIMO(cantBytes, (*p)->tamInfo));

    return 1;
}

int ponerOrdenLista(tLista * pl, const void *pd, unsigned cantBytes, int(*cmp)(const void *, const void *))
{
    tNodo * nue;
    int rc;

    while(*pl && (rc=cmp(pd, (*pl)->info)) > 0)
        pl = &(*pl)->sig;


    if(*pl && !rc)
        return 0; //Duplicado

    nue = (tNodo*)malloc(sizeof(tNodo));

    if(!nue)
        return 0;

    nue->info = malloc(cantBytes);

    if(!nue->info)
    {
        free(nue);
        return 0;
    }

    nue->tamInfo = cantBytes;
    memcpy(nue->info, pd, cantBytes);
    nue->sig = *pl;
    *pl = nue;

    return 1;
}

int sacarElemLista(tLista * pl, void * pd,unsigned tam, int(*cmp)(const void*, const void*))
{
    tNodo * elim;

    while( *pl && cmp(pd, (*pl)->info) != 0)
        pl = &(*pl)->sig;

    if(!*pl)
        return 0;

    elim = *pl;
    memcpy(pd, elim->info, MINIMO(tam, elim->tamInfo));

    *pl = elim->sig;
    free(elim->info);
    free(elim);

    return 1;
}

int sacarElemOrdLista(tLista * pl, void * pd,unsigned tam, int(*cmp)(const void*, const void*))
{
    tNodo * elim;
    int rc;

    while( *pl && (rc = cmp(pd, (*pl)->info)) > 0)
        pl = &(*pl)->sig;

    if(!*pl || rc)
        return 0;

    elim = *pl;
    memcpy(pd, elim->info, MINIMO(tam, elim->tamInfo));

    *pl = elim->sig;
    free(elim->info);
    free(elim);

    return 1;
}

void recorrerLista(tLista *pl, void (*mostrar)(const void *))
{
    while(*pl)
    {
        mostrar((*pl)->info);
        pl = &(*pl)->sig;
    }
}

void recorrerListaArch(tLista *pl, void (*accion)(void *, void *), void* param)
{
    while(*pl)
    {
        accion((*pl)->info, param);
        pl = &(*pl)->sig;
    }
}

void ordenarSelLista(tLista * pl, int(*cmp)(const void*, const void*))
{
    if(!*pl)
        return ;

    while( (*pl)->sig )
    {
        tNodo ** men = buscar_menor_nodo_lista(pl, cmp);
        if( pl != men )
        {
            intercambiar_info_nodos(pl, men);
            //reinsertar_nodo_menor(pl, men);
        }
        pl = &(*pl)->sig;
    }
}

tNodo ** buscar_menor_nodo_lista(const tLista * pl, int(*cmp)(const void*, const void*))
{
    const tLista * men = pl;
    if(!*pl)
        return NULL;

    pl = &(*pl)->sig;

    while(*pl)
    {
        if(cmp((*pl)->info, (*men)->info) < 0)
        {
            men = pl;
        }
        pl = &(*pl)->sig;
    }

    return (tNodo **)men;
}

void reinsertar_nodo_menor(tLista * pl, tLista * men)
{
    tNodo * aux = *men;
    *men = aux->sig;

    aux->sig = *pl;
    *pl = aux;
}

void intercambiar_info_nodos(tLista * n1, tLista * n2)
{
    unsigned aux_tam;
    void * aux = (*n1)->info;
    (*n1)->info = (*n2)->info;
    (*n2)->info = aux;

    aux_tam = (*n1)->tamInfo;
    (*n1)->tamInfo = (*n2)->tamInfo;
    (*n2)->tamInfo = aux_tam;
}

int buscarClaveInt(const tLista * pl, int clave, int(*cmp)(const int, const void*))
{
    if(!*pl)
        return 0;

    while(*pl)
    {
        if( cmp(clave, (*pl)->info) )
            pl = &(*pl)->sig;
        else
            return 1;
    }
    return 0;
}

tNodo ** buscarNodoClave(const tLista * pl, int clave, int(*cmp)(const int, const void*))
{
    if(!*pl)
        return NULL;

    const tLista * busc = pl;

    while(*pl)
    {
        if(cmp(clave, (*pl)->info) == 1)
        {
            busc = pl;
            break;
        }
        else
            pl = &(*pl)->sig;
    }

    return (tNodo**) busc;
}

void eliminarDuplicados(tLista * pl, void * pd, FILE * arch)
{
    tLista * actualPL = pl;
    int clave;
    size_t tam = sizeof(t_producto);

    while(*pl)
    {
        t_producto * prod = (t_producto*)(*pl)->info;
        clave = prod->codProd;
        pl = &(*pl)->sig;
        actualPL = pl;

        while(*pl)
        {
            if(clave == prod->codProd)
            {
                tNodo * elim = * pl;

                memcpy(pd, elim->info, MINIMO(tam, elim->tamInfo));
                *pl = elim->sig;
                free(elim->info);
                free(elim);

                t_producto * prodAG = (t_producto*)pd;

                fprintf(arch, "%4d | %s | %02d/%02d/%4d | %02d/%02d/%4d | %02d | %3.2f | %3.2f\n",
                        prodAG->codProd,
                        prodAG->desc,
                        prodAG->fec_compra.d,
                        prodAG->fec_compra.m,
                        prodAG->fec_compra.a,
                        prodAG->fec_venc.d,
                        prodAG->fec_venc.m,
                        prodAG->fec_venc.a,
                        prodAG->cant,
                        prodAG->prec_compra,
                        prodAG->prec_venta
                       );
            }
            else
                pl = &(*pl)->sig;
        }
        pl = actualPL;
    }
}
