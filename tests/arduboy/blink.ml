module Arduboy = struct
  external init    : unit   -> unit = "ocaml_arduboy_init"
  external print_int : int -> unit = "ocaml_arduboy_print_int"
  external display : unit   -> unit = "ocaml_arduboy_display"
end;;

(*Arduboy.init ();; *)

(* let facto x = *)
(*   let rec aux x acc = *)
(*     match x with *)
(*     | 0 -> acc *)
(*     | _ -> aux (x-1) (acc*x) *)
(*   in *)
(*   aux x 1 *)

(* type 'a ref = { mutable contents : 'a } *)


let rec facto x =
  if x = 0 then 1
  else
    (facto (x-1)) * x

let () =
  Arduboy.init();
  Arduboy.print_int (facto 7);
  Arduboy.display();
