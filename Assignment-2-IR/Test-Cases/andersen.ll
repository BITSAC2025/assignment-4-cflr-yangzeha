; ModuleID = 'andersen.c'
source_filename = "andersen.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.S = type { ptr, ptr }

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 {
entry:
  %s = alloca %struct.S, align 8
  %a1 = alloca i32, align 4
  %a2 = alloca i32, align 4
  %p = alloca ptr, align 8
  %q = alloca ptr, align 8
  %f1 = getelementptr inbounds %struct.S, ptr %s, i32 0, i32 0
  store ptr %a1, ptr %f1, align 8
  %f2 = getelementptr inbounds %struct.S, ptr %s, i32 0, i32 1
  store ptr %a2, ptr %f2, align 8
  %f11 = getelementptr inbounds %struct.S, ptr %s, i32 0, i32 0
  %0 = load ptr, ptr %f11, align 8
  store ptr %0, ptr %p, align 8
  %f22 = getelementptr inbounds %struct.S, ptr %s, i32 0, i32 1
  %1 = load ptr, ptr %f22, align 8
  store ptr %1, ptr %q, align 8
  ret i32 0
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 16.0.4 (https://github.com/bjjwwang/LLVM-compile 2259018fffaba36b5e50dce52fb5aa97b417ce4f)"}
