main() {
  const int m = 26;
  char map[m*m], nei[m*m];
  int x,y;
  for (x = 0; x < m*m; x++) { map[x] = 0; nei[x] = 0; }
  for(;;) {
    int q,w,n;
    char leg='a';
    puts(" abcdefghijklmnopqrstuvwxy   z to step");
    putchar(leg++);
    for (q = 1; q < m-1; q++) {
      for (w = 1; w < m-1; w++) {
        putchar(map[q + w * m]?'*':'-');}
      putchar('\n');
      if (q != m - 2) putchar(leg++);}
    do x = getchar(); while (x == 13 || x == 10);
    if (x == 'z') {
      for (q = 1; q < m-1; q++) {
        for (w = 1; w < m-1; w++) {
          n = 0;
          n += map[q-1 + (w-1) * m];
          n += map[q+1 + (w+1) * m];
          n += map[q-1 + (w+1) * m];
          n += map[q+1 + (w-1) * m];
          n += map[q-1 + (w-0) * m];
          n += map[q+1 + (w+0) * m];
          n += map[q-0 + (w-1) * m];
          n += map[q+0 + (w+1) * m];
          nei[q + w * m] = n;}}
      for (q = 1; q < m-1; q++) {
        for (w = 1; w < m-1; w++) {
          int nn  = nei[q + w * m];
          if (map[q + w * m] == 1) {
            if (nn > 3) map[q + w * m] = 0;
            else if (nn < 2) map[q + w * m] = 0;} 
          else {
            if (nn == 3) map[q + w * m] = 1;}}}
      continue;}
    do y = getchar(); while (y == 13 || y == 10);
    x = x - 'a' + 1;
    y = y - 'a' + 1;
    map[x + y * m] = map[x + y * m] ? 0 : 1;}}
