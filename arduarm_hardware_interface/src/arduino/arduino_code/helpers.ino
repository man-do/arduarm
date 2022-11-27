// helper methods

void cmd_all(int8_t s1, int8_t s2, int8_t s3) {
  int8_t current[NR_SRV];
  int8_t goal[NR_SRV] = {s1, s2, s3};
  int8_t diff[NR_SRV];

  while (*current != *goal) {
    current[0] = servos[0].read();
    current[1] = servos[1].read();
    current[2] = servos[2].read();
    
    diff[0] = goal[0] - current[0];
    diff[1] = goal[1] - current[1];
    diff[2] = goal[2] - current[2];

    servos[0].writeMicroseconds(toMicros(current[0] += sgn(diff[0])));
    servos[1].writeMicroseconds(toMicros(current[1] += sgn(diff[1])));
    servos[2].writeMicroseconds(toMicros(current[2] += sgn(diff[2])));

    joint_position[0] = s1;
    joint_position[1] = s2;
    joint_position[2] = s3;

    delay(1);
  }
}

int sgn(int8_t x) {
  if (x > 0) return 1;
  if (x < 0) return -1;
  return 0;
}

int toMicros(int8_t angle) {
  return map(angle, 0, 180, START, END);
}
