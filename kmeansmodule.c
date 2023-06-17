# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# define PY_SSIZE_T_CLEAN
# include <Python.h>

double distance(double *p, double *q, int d)
{
    int i;
    double distance = 0;
    double sum_of_squares = 0;
    for(i=0; i<d; i++)
    {
        sum_of_squares += pow(p[i]-q[i],2);
    }
    distance = sqrt(sum_of_squares);
    return distance;
}

int clusterAssign(double *x, double **centroids, int k, int d)
{
    double min_distance = distance(x, centroids[0],d);
    int cluster = 0;
    int i;
    double dist;
    for(i=1; i<k; i++)
    {
        dist = distance(x, centroids[i],d);
        if(dist <= min_distance)
        {
            min_distance = dist;
            cluster = i;
        }
    }
    return cluster;
}


void centroidUpdate(double **clusters, int *size_of_clusters, int k, int d)
{
    int i = 0;
    int j = 0;
    for(i=0; i < k; i++)
    {   
        for(j=0; j<d ; j++)
        {
            if(size_of_clusters[i]==0){
                printf("An Error Has Occurred\n");
                exit(1);
            }
            clusters[i][j] /= size_of_clusters[i];
        }
    }
    return;
}


/*int main(int argc, char **argv)
{ 
    int k;
    int iter; 
    int num_of_elements=0;
    int d=0;
    int is_end_of_line = 0;
    double *elements_1d;
    double **elements;
    double *centroids_1d;
    double **centroids;
    double *clusters_1d;
    double **clusters;
    int *size_of_clusters;
    int assigned_centroid;
    
    struct vector *head_vec, *curr_vec;
    struct cord *head_cord, *curr_cord;
    int i, j, l;
    double n;
    char c;
    struct vector *loop_vector;
    struct cord *loop_cord;

    int end = 0;*/

static PyObject* fit(PyObject *self, PyObject *args){
    PyObject *elements_lst;
    PyObject *element;
    PyObject *centroids_lst;
    PyObject *centroid;
    //double **elements;
    //double **centroids;
    int k;
    int iter;
    double eps;
    int num_of_elements;
    int d;
    if(!PyArg_ParseTuple(args, "OOiidii", &elements_lst, &centroids_lst, &k, &iter, &eps, &num_of_elements, &d)) {
        return NULL; /* In the CPython API, a NULL value is never valid for a
                    PyObject* so it is used to signal that an error has occurred. */
    }
    double elements[num_of_elements][d];
    double centroids[k][d];

    for(i=0; i<num_of_elements; i++){
        element = PyList_GetItem(elements_lst, i);
        for(j=0; j<d; j++){
            element_coord = PyFloat_AsDouble(PyList_GetItem(element, j)); 
            elements[i][j] = element_coord;
        }
    }

    for(i=0; i<k; i++){
        centroid = PyList_GetItem(centroids_lst, i);
        for(j=0; j<d; j++){
            centroid_coord = PyFloat_AsDouble(PyList_GetItem(centroid, j));
            centroids[i][j] = centroid_coord;
        }
    }
    double **new_centroids = kMeans(elements, centroids, k, iter, eps, num_of_elements, d);
    PyObject* matrix;
    PyObject* vector;
    PyObject* python_float;
    matrix = PyList_New(num_of_elements);
    for (int i = 0; i < num_of_elements; i++)
    {   
        vector = PyList_New(k);
        for(j=0; j<d; j++){
            python_float = PyFloat_FromDouble(new_centroids[i][j]);
            PyList_SetItem(vector, j, python_float); 
        }
        PyList_SetItem(matrix, i, vector);
    }
    return Py_BuildValue("O", matrix); 
}

static PyMethodDef kmeansMethods[] = {
{   "fit",                   /* the Python method name that will be used */
    (PyCFunction) fit, /* the C-function that implements the Python function and returns static PyObject*  */
    METH_VARARGS,           /* flags indicating parameters accepted for this function */
    PyDoc_STR("Performs k-means algorithem on given elements and centroids, and returns the final centriods.
    expected arguments: elements- a 2D list of d dimentional points, of type float.
                        centriods- a 2D list of d dimentional points, of type float. They are k random points from elements.
                        k- the number of centriods. int.
                        iter- the number of iterations. int.
                        eps- the epsilon value for convergence. float.
                        num_of_elements- the number of points in elements. int.
                        d- number of coordinates of each point. int. ")}, /*  The docstring for the function */
    {NULL, NULL, 0, NULL}     /* The last entry must be all NULL as shown to act as a
                                sentinel. Python looks for this entry to know that all
                                of the functions for the module have been defined. */
};

static struct PyModuleDef kmeansmodule = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp", /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,  /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    kmeansMethods /* the PyMethodDef array from before containing the methods of the extension */
};

PyMODINIT_FUNC PyInit_mykmeanssp(void){
    PyObject *m;
    m = PyModule_Create(&kmeansmodule);
    if (!m) {
        return NULL;
    }
    return m;
}

/*memory allocation for sum matrix "representing" clusters
clusters_1d = calloc(k*d, sizeof(double));
if(clusters_1d == NULL){
    printf("An Error Has Occurred\n");
    delete_vector(head_vec);
    free(elements_1d);
    free(elements);
    free(centroids_1d);
    free(centroids);
    exit(1);
}

clusters = calloc(k,sizeof(double *));
if(clusters == NULL){
    printf("An Error Has Occurred\n");
    delete_vector(head_vec);
    free(elements_1d);
    free(elements);
    free(centroids_1d);
    free(centroids);
    free(clusters_1d);
    exit(1);
}
for(j=0; j<k; j++)
{
    clusters[j] = clusters_1d+j*d;
} 

/*memory allocation for array of sizes of clusters
size_of_clusters = calloc(k, sizeof(int));
if(size_of_clusters == NULL){
    printf("An Error Has Occurred\n");
    delete_vector(head_vec);
    free(elements_1d);
    free(elements);
    free(centroids_1d);
    free(centroids);
    free(clusters_1d);
    free(clusters);
    exit(1);
}

/*assignment of elements into 2d matrix
i=0;
j=0;
loop_vector = head_vec;
for(i=0; i<num_of_elements; i++){
    loop_cord = loop_vector->cords;
    for(j=0; j<d; j++){
        elements[i][j]= loop_cord->value;
        loop_cord=loop_cord->next;
    }
    loop_vector=loop_vector->next;
}
delete_vector(head_vec);
*/
double** kMeans(double **elements, double **centroids, int k, int iter, double eps, int num_of_elements, int d){
    int end = 0;
    double *clusters_1d;
    double **clusters;
    int *size_of_clusters;
    int assigned_centroid;
    int i, j, l;

    /*memory allocation for sum matrix "representing" clusters*/
    clusters_1d = calloc(k*d, sizeof(double));
    if(clusters_1d == NULL){
        printf("An Error Has Occurred\n");
        exit(1);
    }

    clusters = calloc(k,sizeof(double *));
    if(clusters == NULL){
        printf("An Error Has Occurred\n");
        free(clusters_1d);
        exit(1);
    }
    for(j=0; j<k; j++)
    {
        clusters[j] = clusters_1d+j*d;
    } 

    /*memory allocation for array of sizes of clusters*/
    size_of_clusters = calloc(k, sizeof(int));
    if(size_of_clusters == NULL){
        printf("An Error Has Occurred\n");
        free(clusters_1d);
        free(clusters);
        exit(1);
    }
    
    for(i=0; i<iter; i++){ 
        for(j=0; j<num_of_elements; j++){ 
            assigned_centroid= clusterAssign(elements[j], centroids,k,d);
            for(l=0; l<d; l++){
                clusters[assigned_centroid][l] += elements[j][l];
            }
            size_of_clusters[assigned_centroid]+=1;
        }
        centroidUpdate(clusters, size_of_clusters, k, d);
        j=0;
        end=1;
        for(j=0; j<k; j++){
            if(distance(centroids[j],clusters[j],d)>= eps){
                end = 0;
                break;
            }
    
        }
        if(end){
            break;
        }
        j=0;
        l=0;
        for(j=0;j<k;j++){
            for(l=0;l<d;l++){
                centroids[j][l] = clusters[j][l];
                clusters[j][l]=0;
            }
            size_of_clusters[j]=0;
        }
    }

    /*free(elements);
    free(centroids);
    free(centroids_1d);
    free(elements_1d);*/
    free(clusters);
    free(size_of_clusters);
    free(clusters_1d);

    return centroids;
}
    /*i=0;
    j=0;
    l=0;
    for(i=0; i<iter; i++)
    { 
       for(j=0; j<num_of_elements; j++){ 
        assigned_centroid= clusterAssign(elements[j], centroids,k,d);
        for(l=0; l<d; l++){
            clusters[assigned_centroid][l] += elements[j][l];
        }
        size_of_clusters[assigned_centroid]+=1;
       }
       centroidUpdate(clusters, size_of_clusters, k, d);
       j=0;
       end=1;
       for(j=0; j<k; j++){
        if(distance(centroids[j],clusters[j],d)>= eps){
            end = 0;
            break;
        }
       
       }
       if(end){
        break;
       }
       j=0;
       l=0;
       for(j=0;j<k;j++){
        for(l=0;l<d;l++){
            centroids[j][l] = clusters[j][l];
            clusters[j][l]=0;
        }
        size_of_clusters[j]=0;
       }
    }

    /*free(elements);
    free(centroids);
    free(centroids_1d);
    free(elements_1d);
    free(clusters);
    free(size_of_clusters);
    free(clusters_1d);

    return centroids;
    //return 0;*/
//}