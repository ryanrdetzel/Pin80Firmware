
// Convert hex string to int value
int x2i(char *s){
 int x = 0;
 for(;;) {
   char c = *s;
   if (c >= '0' && c <= '9') {
      x *= 16;
      x += c - '0';
   }
   else if (c >= 'A' && c <= 'F') {
      x *= 16;
      x += (c - 'A') + 10;
   }
   else break;
   s++;
 }
 return x;
}
