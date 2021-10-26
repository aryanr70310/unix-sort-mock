#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>

int string_compare(const void *a, const void *b);

int strcmp_ic(char const *a, char const *b);

int nstrcmp_ic(char const *a, char const *b);

int n_string_compare(const void *a, const void *b);

int rev_string_compare(const void *a, const void *b);

int revn_string_compare(const void *a, const void *b);

int main(int argc, char *argv[]){
  FILE *fp;
  char c;
  int chk_arr[]={0,0,0,0,1};
  int i=0;int j=0;int k=0;
  int n_files=0;
  for (i=1;i<argc;i++){ // Parsing the arguments
    if (argv[i][0] == '-' ){
      switch (argv[i][1])  {
        default:
          printf("Unknown option : %c \n\n", argv[0][1]);
          return 0;
        case 'r': // reverse
          if (chk_arr[0]==0){
            chk_arr[4]=0;
            chk_arr[0]=1;}
          continue;
        case 'h': // info
          if (chk_arr[1]==0){
            chk_arr[1]=1;}
          continue;
        case 'n': // numeric
          if (chk_arr[2]==0){
            chk_arr[4]=0;
            chk_arr[2]=1;}
          continue;
        case 'o': // file writer
          if (chk_arr[3]==0){
            chk_arr[3]=1;}
          else{
            n_files++;
            chk_arr[3]++;
            }
          }
          continue;
      }
    else{
      n_files++;
      }
    }
  if (n_files==0 && chk_arr[3]>0){
    printf("No output file is specified for -o option");
    return 0;
  }
  else if(n_files==0 || (n_files==1 && chk_arr[3]>0)){ // for reading from stdin
    char line[100];
    fp=fopen("sorts_buffer.txt","w"); // buffer file , deleted after being used
    int long_line=0;
    int num_lines=0;
    i=0;j=0;
    printf("\n");
    while (fgets(line, sizeof(line), stdin)!=NULL) { // reading from stdin
      fputs(line,fp);
      if ((int)strlen(line)>long_line){
        long_line=strlen(line);
      }
      num_lines++;
    }
    fclose(fp);
    long_line+=num_lines;
    char strings[num_lines][long_line];
    fp=fopen("sorts_buffer.txt","r"); // storing elements in strings
    while (fgets(strings[i], sizeof(strings[i]), fp)) {
      strings[i][strcspn(strings[i], "\n\r")] = 0;
      i++;
    }
    fclose(fp);
    remove("sorts_buffer.txt");
    if (chk_arr[4]==1){
      qsort(strings,sizeof(strings)/long_line, long_line,string_compare); // default, no options
    }
    else if (chk_arr[2]==1 && chk_arr[0]==0){
      qsort(strings,sizeof(strings)/long_line, long_line,n_string_compare); // numeric
    }
    else if (chk_arr[2]==1 && chk_arr[0]==1){
      qsort(strings,sizeof(strings)/long_line, long_line,revn_string_compare); // reverse numeric
    }
    else if (chk_arr[0]==1){
      qsort(strings,sizeof(strings)/long_line, long_line,rev_string_compare); // reverse
    }
    if(chk_arr[3]>1){ // writes to -o file
      fp=fopen("-o","w");
      for(i=0;i<num_lines;i++){
        fputs(strings[i],fp);
        fputs("\n",fp);
      }
      fclose(fp);
    }
    else if (chk_arr[3]>0){ // writes to specified file
      for(i=1;i<argc;i++){
        if(argv[i][0]!='-'){
          fp=fopen(argv[i],"w");
        }
      }
      for(i=0;i<num_lines;i++){
        fputs(strings[i],fp);
        fputs("\n",fp);
      }
      fclose(fp);
    }
    else { // writes to stdout
      for(i=0;i<num_lines;i++){
        printf("\n%s",strings[i]);
      }
      printf("\n");
    }
  }
  else { // reading from file(s)
    int num_lines=0;
    int long_line=0;
    int o_chk;
    if (chk_arr[3]>0){
      o_chk=-1;
    }
    else{
      o_chk=-2;
    }
    for (i=1;i<argc;i++){ // reads from files
      if (argv[i][0] != '-' ){
        if (o_chk==-1){
          o_chk=i;
          continue;
        }
        fp=fopen(argv[i],"r");
        for (c = getc(fp); c != EOF; c = getc(fp)){
          if (c=='\n'){
            num_lines++;
            if (j>long_line){
              long_line=j;
            }
            j=0;
          }
          else{
            j++;
          }
        }
        num_lines+=1;
        fclose(fp);
      }
      else{
        j=0;
        if(chk_arr[3]>1 && argv[i][1] == 'o' ){
          if (o_chk==-1){
          o_chk=i;
          continue;
        }
          fp=fopen(argv[i],"r");
          for (c = getc(fp); c != EOF; c = getc(fp)){
            if (c=='\n'){
              num_lines++;
              if (j>long_line){
                long_line=j;
              }
              j=0;
            }
            else{
              j++;
            }
          }
          chk_arr[3]--;
          k++;
          num_lines+=1;
          fclose(fp);
        }
      }
    }
    long_line+=num_lines;
    char strings[num_lines][long_line];
    i=0;j=0;chk_arr[3]+=k;
    for (k=1;k<argc;k++){ // stores strings within strings
      if (k==o_chk){
        continue;
      }
      if (argv[k][0] != '-' ){
        fp=fopen(argv[k],"r");
         while (fgets(strings[i], sizeof(strings[i]), fp)) {
           strings[i][strcspn(strings[i], "\n\r")] = 0;
           i++;
         }
         fclose(fp);
      }
      else {
        if(chk_arr[3]>1 && argv[k][1] == 'o' ){
          fp=fopen(argv[k],"r");
          while (fgets(strings[i], sizeof(strings[i]), fp)) {
            i++;
          }
          chk_arr[3]--;
          fclose(fp);
        }
      }
    }
    num_lines=i+1;
    printf("\n");
    if (chk_arr[4]==1){
      qsort(strings,sizeof(strings)/long_line, long_line,string_compare); // default, no options
    }
    else if (chk_arr[2]==1 && chk_arr[0]==0){
      qsort(strings,sizeof(strings)/long_line, long_line,n_string_compare); // numeric
    }
    else if (chk_arr[2]==1 && chk_arr[0]==1){
      qsort(strings,sizeof(strings)/long_line, long_line,revn_string_compare); // reverse numeric
    }
    else if (chk_arr[0]==1){
      qsort(strings,sizeof(strings)/long_line, long_line,rev_string_compare); // reverse
    }
    if(o_chk>0){ // writes to specified file
      fp=fopen(argv[o_chk],"w");
      for(i=0;i<num_lines;i++){
        fputs(strings[i],fp);
        fputs("\n",fp);
      }
      fclose(fp);
    }
    else { // prints to stdout
      for(i=0;i<num_lines;i++){
        printf("\n%s",strings[i]);
      }
      printf("\n");
    }
  }
  if (chk_arr[1]==1){
    printf("\nsort.c can read arrays from either stdin, or specified file(s).");
    printf("\nIt supports all combinations of the -o,-h,-r,-n options and any number of files ");
    printf("\n ,supplied through the command line. The lines read from stdin are");
    printf("\nlimited to 100 characters, but length of lines from input files is unrestricted.");
    printf("\nWhile using stdin, the program requires you to click enter and then Ctrl+D to stop typing.");
    printf("\nThe program doesn't support files starting with - except for -o.");
  }
}

int string_compare(const void *a, const void *b) { // compare function
  return strcmp_ic(a,b);
} 

int rev_string_compare(const void *a, const void *b) { // compare function
  return strcmp_ic(b,a);
} 

int n_string_compare(const void *a, const void *b) { // compare function
  const char *sa = (const char *) a;
  const char *sb = (const char *) b;
  int ia = atoi(sa);
  int ib = atoi(sb);

  if (ia < ib){ 
    return -1;
    }
  if (ia > ib){ 
    return 1;
  }
  return strcmp_ic(a, b);
}

int revn_string_compare(const void *a, const void *b) { // compare function
  const char *sa = (const char *) a;
  const char *sb = (const char *) b;
  int ia = atoi(sa);
  int ib = atoi(sb);

  if (ia < ib){ 
    return 1;
    }
  if (ia > ib){ 
    return -1;
  }
  return strcmp_ic(b, a);
}

int strcmp_ic(char const *a, char const *b){
  for (;; a++, b++) {
    int chk = tolower((unsigned char)*a) - tolower((unsigned char)*b);
    if (chk != 0 || !*a){
        return chk;
    }
  }
}