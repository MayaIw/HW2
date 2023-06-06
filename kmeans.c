# include <stdio.h>
# include <stdlib.h>
# include <math.h>

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


int main(int argc, char **argv)
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

    int end = 0;

    
    /*memory allocation for sum matrix "representing" clusters*/
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

    /*memory allocation for array of sizes of clusters*/
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
    
    /*assignment of elements into 2d matrix*/
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
    
    i=0;
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
        if(distance(centroids[j],clusters[j],d)>= 0.001){
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

    
    printClusters(centroids, d, k);

    free(elements);
    free(centroids);
    free(centroids_1d);
    free(elements_1d);
    free(clusters);
    free(size_of_clusters);
    free(clusters_1d);

    
    return 0;
}