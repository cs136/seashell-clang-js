; ModuleID = 'seashell-compiler-output'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 0, i32* %1
  store i32 0, i32* %a, align 4
  store i32 2, i32* %b, align 4
  store i32 3, i32* %c, align 4
  %2 = load i32* %b, align 4
  %3 = call i32 (...)* @_suspend()
  %4 = add nsw i32 %2, %3
  %5 = call i32 (...)* @_suspend()
  %6 = add nsw i32 %4, %5
  %7 = load i32* %c, align 4
  %8 = add nsw i32 %6, %7
  store i32 %8, i32* %a, align 4
  %9 = load i32* %a, align 4
  ret i32 %9
}

define void @_start() {
  %1 = call i32 @main()
  call void @_exit(i32 %1)
  ret void
}

declare void @_exit(i32)

declare i32 @_suspend(...) #1

attributes #0 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-realign-stack" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-realign-stack" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.6.1 (http://llvm.org/git/clang.git f06a0c83eb2f36bf182224a127ad378f42ab5094) (http://llvm.org/git/llvm.git 0ee768621b2f5c1ece736c42a9a32e29083f35ee)"}

