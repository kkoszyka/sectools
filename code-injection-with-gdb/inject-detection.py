#!/usr/bin/python

from bcc import BPF

# define BPF program
bpf_program = """
int ptrace_attach_event(void *ctx) {
  bpf_trace_printk("PTRACE_ATTACH detected!\\n");
  return 0;
}
int ptrace_stop_event(void *ctx) {
  bpf_trace_printk("PTRACE_STOP detected!\\n");
  return 0;
}
"""

# load BPF program
b = BPF(text=bpf_program)

#attach kprobes for ptrace_attach and ptrace_stop
b.attach_kprobe(event="ptrace_attach", fn_name="ptrace_attach_event")
b.attach_kprobe(event="ptrace_stop", fn_name="ptrace_stop_event")

#Console output
print("TS              PID     TASK    MSG")

while True:
  try:
    (task, pid, cpu, flags, ts, msg) = b.trace_fields()
  except ValueError:
    continue
  except KeyboardInterrupt:
    print("CU")
    break
  print("%f\t%d\t%s\t%s" % (ts, pid, task, msg))
