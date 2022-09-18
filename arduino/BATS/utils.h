static inline int8_t sgn(long val) {
  if (val < 0) return -1;
  if (val==0) return 0;
  return 1;
}
