let rec interval min max =
  if min > max then [] else min :: interval (min + 1) max


let remove_multiples_of n =
  List.filter (fun m -> not ((m mod n) = 0))

(* The sieve itself *)

let sieve max =
  let rec filter_again = function
     [] -> []
    | n::r as l ->
      if n*n > max then l else n :: filter_again (remove_multiples_of n r)
  in
    filter_again (interval 2 max)


let _ =
  let open Avr in
  Serial.init ();
  Serial.write_string "START";
  (* let n = millis () in *)
  begin
    try
        for i = 0 to 10 do
          (* Serial.write_int i; *)
          (* Serial.write '-'; *)
          (* Gc.run (); *)
          (* Serial.write_int (Gc.live_words ()); *)
          let l = sieve 100 in
          List.iter (fun x -> Serial.write_int x ; Serial.write ',') l;
          (* Serial.write '-'; *)
          (* Serial.write_int (Gc.used_stack_size ()); *)
            (* Serial.write '\n'; *)
        done
  with Stack_overflow -> Serial.write_string "STACKOVERFLOW\n"
     | Out_of_memory ->
        Serial.write_string "OUTOFMEMORY\n";
     | _ -> Serial.write '?'
end;
(* let n' = millis () in *)
  (* Serial.write_int (n'-n); *)
  Serial.write_int (Gc.collections ());
Serial.write_string "STOP";
