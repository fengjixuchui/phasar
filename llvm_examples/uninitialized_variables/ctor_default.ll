; ModuleID = 'ctor_default.cpp'
source_filename = "ctor_default.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.MyType = type { i32 }

$_ZN6MyTypeC2Ev = comdat any

; Function Attrs: norecurse uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca %struct.MyType, align 4
  store i32 0, i32* %1, align 4
  call void @_ZN6MyTypeC2Ev(%struct.MyType* %2)
  ret i32 0
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN6MyTypeC2Ev(%struct.MyType*) unnamed_addr #1 comdat align 2 {
  %2 = alloca %struct.MyType*, align 8
  store %struct.MyType* %0, %struct.MyType** %2, align 8
  %3 = load %struct.MyType*, %struct.MyType** %2, align 8
  ret void
}

attributes #0 = { norecurse uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.9.0-svn274465-1~exp1 (trunk)"}
