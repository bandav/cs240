/* Vindhya Banda, hw13.c, CS 24000, Spring 2020
 * Last updated May 1, 2020
 */

 #include "object.h"
 #include "graphics.h"

 #include <stdio.h>
 #include <stdlib.h>
 #include <assert.h>

/*
 * Reads object from file and dynamically allocated
 * memory for it.
 */

object *read_object(const char *filename) {
  if (filename == NULL) {
    return NULL;
  }

  //Open the file
  FILE *in_file = fopen(filename, "r");
  if (in_file == NULL) {
    fclose(in_file);
    in_file = NULL;
    return NULL;
  }

  int result = 0;

  //Read the name
  result = fscanf(in_file, "%*s\n");
  if (result != 0) {
    fclose(in_file);
    in_file = NULL;
    return NULL;
  }
  printf("Result: %d\n", result);

  //Allocate an object
  object *new_obj = malloc(sizeof(object)); //cast?
  if (new_obj == NULL) {
    fclose(in_file);
    in_file = NULL;
    return NULL;
  }

  printf("Executed till 48\n");
  //Read the number of points and polygons
  result = fscanf(in_file, "%d %d\n", &(new_obj->num_points), &(new_obj->num_polygons));
  printf("executed after first fscanf\n");

  if ((result != 2) || (new_obj->num_points == 0) || (new_obj->num_polygons == 0)) {
    fclose(in_file);
    in_file = NULL;
    free_object(new_obj);
    return NULL;
  }

  //Allocate an array of points within the object
  new_obj->points = malloc(new_obj->num_points * sizeof(point));
  if (new_obj->points == NULL) {
    fclose(in_file);
    in_file = NULL;
    free_object(new_obj);
    return NULL;
  }

  //Read each point into the array of points
  int start_index = 0;

  result = fscanf(in_file, "%d %lf %lf %lf\n",
                     &start_index,
                     &new_obj->points[0].arr[0],
                     &new_obj->points[0].arr[1],
                     &new_obj->points[0].arr[2]);
    new_obj->points[0].arr[3] = 0;
    if (result != 4) {
      fclose(in_file);
      in_file = NULL;
      free_object(new_obj);
      return NULL;
    }

  for (int i = 1; i < new_obj->num_points; i++) {
    result = fscanf(in_file, "%*d %lf %lf %lf\n",
                     &new_obj->points[i].arr[0],
                     &new_obj->points[i].arr[1],
                     &new_obj->points[i].arr[2]);
    if (result != 3) {
      fclose(in_file);
      in_file = NULL;
      free_object(new_obj);
      return NULL;
    }
    new_obj->points[i].arr[3] = 0;
  }

  //Allocate an array of polygons
  new_obj->polygons = malloc(new_obj->num_polygons * sizeof(polygon));
  if (new_obj->polygons == NULL) {
    fclose(in_file);
    in_file = NULL;
    free_object(new_obj);
    return NULL;
  }

  //Read each polygon into the array of polygons
  for (int j = 0; j < new_obj->num_polygons; j++) {
    //Read the name and throw it away
    result = fscanf(in_file, "%*s");
    if (result != 0) {
      fclose(in_file);
      in_file = NULL;
      free_object(new_obj);
      return NULL;
    }

    //Read the color & convert it to an R, G, B value by calling name_to_rgb()
    char color[8];
    result = fscanf(in_file, "%s", color);
    if (result != 1) {
      fclose(in_file);
      in_file = NULL;
      free_object(new_obj);
      return NULL;
    }

    name_to_rgb(color,
                 &new_obj->polygons[j].r,
                 &new_obj->polygons[j].g,
                 &new_obj->polygons[j].b);

    //Read the number of points
    result = fscanf(in_file, "%d", &new_obj->polygons[j].num_points);
    if (result != 1) {
      fclose(in_file);
      in_file = NULL;
      free_object(new_obj);
      return NULL;
    }

    //Read each of the points and make each entry of the array of pointers
    //in the polygon structure point to a point in the object structure
    new_obj->polygons[j].point_arr = malloc(sizeof(point *) * new_obj->polygons[j].num_points);
    if (new_obj->polygons[j].point_arr == NULL) {
      fclose(in_file);
      in_file = NULL;
      free_object(new_obj);
      return NULL;
    }

    int read_int = 0;

    for (int k = 0; k < new_obj->polygons[j].num_points; k++) {
      result = fscanf(in_file, "%d", &read_int);
      if (result != 1) {
        fclose(in_file);
        in_file = NULL;
        free_object(new_obj);
        return NULL;
      }
      new_obj->polygons[j].point_arr[k] = &new_obj->points[read_int - start_index];
    }
  }
  fclose(in_file);
  in_file = NULL;

  return new_obj;
}

void free_object(object *obj) {
  if (obj == NULL) {
  }
  assert(obj != NULL);
  polygon *temp = obj->polygons;
  for (int i= 0; i < obj->num_polygons; i++) {
    free(temp->point_arr);
    temp->point_arr = NULL;
    temp++;
  }
  free(obj->polygons);
  obj->polygons = NULL;
  free(obj->points);
  obj->points = NULL;
  free(obj);
  obj = NULL;
}
