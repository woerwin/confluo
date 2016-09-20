#include "filter_benchmark.h"

const char* usage =
    "Usage: %s [-b bench-type] [-n numthreads] [-r ratelimit] data-path\n";

void print_usage(char *exec) {
  LOG(stderr, usage, exec);
}

int main(int argc, char** argv) {
  if (argc < 2 || argc > 6) {
    print_usage(argv[0]);
    return -1;
  }

  int c;
  std::string bench_type = "latency-get";
  int num_threads = 1;
  uint64_t rate_limit = 0;
  while ((c = getopt(argc, argv, "b:n:")) != -1) {
    switch (c) {
      case 'b':
        bench_type = std::string(optarg);
        break;
      case 'n':
        num_threads = atoi(optarg);
        break;
      case 'r':
        rate_limit = atoll(optarg);
        break;
      default:
        LOG(stderr, "Could not parse command line arguments.\n");
    }
  }

  if (optind == argc) {
    print_usage(argv[0]);
    return -1;
  }

  std::string data_path = std::string(argv[optind]);
  std::string attr_path = std::string(argv[optind + 1]);

  filter_benchmark ls_bench(data_path, attr_path);
  if (bench_type.find("latency") == 0) {
    if (bench_type == "latency-q1") {
      ls_bench.latency_q1();
    } else if (bench_type == "latency-q2") {
      ls_bench.latency_q2();
    } else if (bench_type == "latency-q3") {
      ls_bench.latency_q3();
    } else if (bench_type == "latency-q4") {
      ls_bench.latency_q4();
    } else if (bench_type == "latency-q5") {
      ls_bench.latency_q5();
    } else if (bench_type == "latency-q6") {
      ls_bench.latency_q6();
    } else {
      ls_bench.latency_all();
    }
  } else if (bench_type == "throughput") {

  } else {
    LOG(stderr, "Unknown benchmark type: %s\n", bench_type.c_str());
  }

  return 0;
}