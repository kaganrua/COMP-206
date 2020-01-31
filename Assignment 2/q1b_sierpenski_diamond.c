#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void print_row(int row, int tri_height, int level)
{
  int tri_width = tri_height * 2 - 1;
  if (level == 1)
  {
    for(int j=0; j < tri_height-1-row; j++){
	printf(" ");
	}
    for(int i = 0; i < (2 * row + 1); i++ )
    {
      printf("*");
    }
    for(int k=0; k < tri_height-1-row; k++){
	printf(" ");
	}
  }
  else if (row < tri_height/2)
  {
    for(int c = 0; c < ceil(tri_width/4.0); c++){
	printf(" ");
	}
    print_row(row, tri_height/2, level-1 );
    for(int a=0; a < ceil(tri_width/4.0); a++){
	printf(" ");
	}
  }
  else
  {     
	 print_row(row - tri_height/2, tri_height/2, level - 1);
    printf(" ");  
    print_row(row - tri_height/2, tri_height/2, level - 1);
  }
}


int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    printf("ERROR: Wrong number of arguments. You must enter two arguments: a Height and a Fractal Level.");
    return 0;
  }
  int height = atoi(argv[1]);
  int level = atoi(argv[2]);
  if(height%2==0 || height < 0)
  {
    printf("ERROR: Height must be a odd positive integer!");
	return 0; 
  }
  int tri_height = ceil( height/2.0 );
 int x = pow(2, (level-1));
  if(tri_height < x)
  {
     printf("ERROR:Height does not allow evenly dividing requested number of levels"); 
 	return 0;
  }

  for(int row = 0; row < tri_height; row++ )
  {
    print_row(row, tri_height, level);
    printf("\n");
  }
  for(int row = tri_height - 2; row >= 0; row-- )
  {
    print_row(row, tri_height, level);
    printf("\n");
  }

}
