
begin
    integer k;
    integer m;
    
    (* factorial *)
    integer function F(n);
    begin
        if n <= 0 then 
            F := 1;
        else 
            F := n * F(n-1);
    end

    (* fabnacci sequence *)
    integer function fa(n);
    begin
        if n = 1 then
            fa := 1;
        else if n = 2 then
            fa := 1;
        else
            fa := fa(n - 1) + fa(n - 2);
    end
    
    read(m);
    write(F(m)); (* SIGFP,ha ha *)
    
    read(m);
    write(fa(m));

    __halt_compiler();

    The following code will not be compiled.

    rfnj

    hkm

    soor
end
