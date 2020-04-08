
(* Tools *)

let int_of_hex1 c =
  match c with
  | '0' .. '9' -> int_of_char c - int_of_char '0'
  | 'a' .. 'f' -> int_of_char c - int_of_char 'a' + 10
  | 'A' .. 'F' -> int_of_char c - int_of_char 'A' + 10
  | _ -> invalid_arg "Simul.int_of_hex1"
let int_of_hex2 c1 c0 =
  (int_of_hex1 c1 lsl 4) lor int_of_hex1 c0
;;

let int_of_hex3 c2 c1 c0 =
  (int_of_hex1 c2 lsl 8) lor (int_of_hex1 c1 lsl 4) lor int_of_hex1 c0
;;

(***)

type port = PORTA | PORTB | PORTC | PORTD | PORTE | PORTF | SPDR | PORTG | PORTH | PORTI | PORTJ | PORTK | PORTL

let string_of_port port =
  match port with
  | PORTA -> "PORTA"
  | PORTB -> "PORTB" | PORTC -> "PORTC"
  | PORTD -> "PORTD" | PORTE -> "PORTE" | PORTF -> "PORTF"
  | PORTG -> "PORTG"
  | PORTH -> "PORTH"
  | PORTI -> "PORTI"
  | PORTJ -> "PORTJ"
  | PORTK -> "PORTK"
  | PORTL -> "PORTL"
  | SPDR -> "SPDR"
;;

let port_of_string s =
  match String.uppercase_ascii s with
  | "PORTA" -> PORTA
  | "PORTB" -> PORTB | "PORTC" -> PORTC
  | "PORTD" -> PORTD | "PORTE" -> PORTE | "PORTF" -> PORTF
  | "SPDR" -> SPDR
  | _ -> invalid_arg "Simul.port_of_string"
;;

let char_of_port port =
  match port with
  | PORTA -> 'A'
  | PORTB -> 'B' 
  | PORTC -> 'C' 
  | PORTD -> 'D' 
  | PORTE -> 'E' 
  | PORTF -> 'F' 
  | SPDR -> 'G'
  | PORTG -> 'G'
  | PORTH -> 'H'
  | PORTI -> 'I'
  | PORTJ -> 'J'
  | PORTK -> 'K'
  | PORTL -> 'L'
;;

let port_of_char c =
  match c with
    | 'B' | 'b' -> PORTB
    | 'C' | 'c' -> PORTC
    | 'D' | 'd' -> PORTD 
    | 'E' | 'e' -> PORTE 
    | 'F' | 'f' -> PORTF
    | 'G' | 'g' -> SPDR
    | 'H' | 'h' -> PORTH
    | 'I' | 'i' -> PORTI
    | 'J' | 'j' -> PORTJ
    | 'K' | 'k' -> PORTK
    | 'L' | 'l' -> PORTL
    | _ -> invalid_arg "Simul.port_of_char"
;;

let index_of_port port =
  match port with
  | PORTA -> 0
  | PORTB -> 1
  | PORTC -> 2
  | PORTD -> 3
  | PORTE -> 4
  | PORTF -> 5
  | SPDR -> 6
  | PORTG -> 7
  | PORTH -> 8
  | PORTI -> 9
  | PORTJ -> 10
  | PORTK -> 11
  | PORTL -> 12
;;

let port_of_index ind =
  match ind with
    | 1 -> PORTB | 2 -> PORTC | 3 -> PORTD | 4 -> PORTE | 5 -> PORTF | 6 -> SPDR
    | _ -> invalid_arg "Simul.port_of_index"
;;

(***)

type pin =
  | RA0 | RA1 | RA2 | RA3 | RA4 | RA5 | RA6 | RA7
  | RB0 | RB1 | RB2 | RB3 | RB4 | RB5 | RB6 | RB7
  | RC0 | RC1 | RC2 | RC3 | RC4 | RC5 | RC6 | RC7
  | RD0 | RD1 | RD2 | RD3 | RD4 | RD5 | RD6 | RD7
  | RE0 | RE1 | RE2 | RE3 | RE4 | RE5 | RE6 | RE7
  | RF0 | RF1 | RF2 | RF3 | RF4 | RF5 | RF6 | RF7
  | SPDR0 | SPDR1 | SPDR2 | SPDR3 | SPDR4 | SPDR5 | SPDR6 | SPDR7
  | RG0 | RG1 | RG2 | RG3 | RG4 | RG5 | RG6 | RG7
  | RH0 | RH1 | RH2 | RH3 | RH4 | RH5 | RH6 | RH7
  | RI0 | RI1 | RI2 | RI3 | RI4 | RI5 | RI6 | RI7
  | RJ0 | RJ1 | RJ2 | RJ3 | RJ4 | RJ5 | RJ6 | RJ7
  | RK0 | RK1 | RK2 | RK3 | RK4 | RK5 | RK6 | RK7
  | RL0 | RL1 | RL2 | RL3 | RL4 | RL5 | RL6 | RL7

;;

let string_of_pin pin =
  match pin with
    | RA0 -> "RA0" | RA1 -> "RA1" | RA2 -> "RA2" | RA3 -> "RA3"
    | RA4 -> "RA4" | RA5 -> "RA5" | RA6 -> "RA6" | RA7 -> "RA7"
    | RB0 -> "RB0" | RB1 -> "RB1" | RB2 -> "RB2" | RB3 -> "RB3"
    | RB4 -> "RB4" | RB5 -> "RB5" | RB6 -> "RB6" | RB7 -> "RB7"
    | RC0 -> "RC0" | RC1 -> "RC1" | RC2 -> "RC2" | RC3 -> "RC3"
    | RC4 -> "RC4" | RC5 -> "RC5" | RC6 -> "RC6" | RC7 -> "RC7"
    | RD0 -> "RD0" | RD1 -> "RD1" | RD2 -> "RD2" | RD3 -> "RD3"
    | RD4 -> "RD4" | RD5 -> "RD5" | RD6 -> "RD6" | RD7 -> "RD7"
    | RE0 -> "RE0" | RE1 -> "RE1" | RE2 -> "RE2" | RE3 -> "RE3"
    | RE4 -> "RE4" | RE5 -> "RE5" | RE6 -> "RE6" | RE7 -> "RE7"
    | RF0 -> "RF0" | RF1 -> "RF1" | RF2 -> "RF2" | RF3 -> "RF3"
    | RF4 -> "RF4" | RF5 -> "RF5" | RF6 -> "RF6" | RF7 -> "RF7"

    | RG0 -> "RG0" | RG1 -> "RG1" | RG2 -> "RG2" | RG3 -> "RG3"
    | RG4 -> "RG4" | RG5 -> "RG5" | RG6 -> "RG6" | RG7 -> "RG7"


    | RH0 -> "RH0" | RH1 -> "RH1" | RH2 -> "RH2" | RH3 -> "RH3"
    | RH4 -> "RH4" | RH5 -> "RH5" | RH6 -> "RH6" | RH7 -> "RH7"


    | RI0 -> "RI0" | RI1 -> "RI1" | RI2 -> "RI2" | RI3 -> "RI3"
    | RI4 -> "RI4" | RI5 -> "RI5" | RI6 -> "RI6" | RI7 -> "RI7"


    | RJ0 -> "RJ0" | RJ1 -> "RJ1" | RJ2 -> "RJ2" | RJ3 -> "RJ3"
    | RJ4 -> "RJ4" | RJ5 -> "RJ5" | RJ6 -> "RJ6" | RJ7 -> "RF7"


    | RK0 -> "RK0" | RK1 -> "RK1" | RK2 -> "RK2" | RK3 -> "RK3"
    | RK4 -> "RK4" | RK5 -> "RK5" | RK6 -> "RK6" | RK7 -> "RK7"


    | RL0 -> "RL0" | RL1 -> "RL1" | RL2 -> "RL2" | RL3 -> "RL3"
    | RL4 -> "RL4" | RL5 -> "RL5" | RL6 -> "RL6" | RL7 -> "RL7"



    | SPDR0 -> "SPDR0"
    | SPDR1 -> "SPDR1"
    | SPDR2 -> "SPDR2"
    | SPDR3 -> "SPDR3"
    | SPDR4 -> "SPDR4"
    | SPDR5 -> "SPDR5"
    | SPDR6 -> "SPDR6"
    | SPDR7 -> "SPDR7"
;;

let port_of_pin pin =
  match pin with
  | RA0 | RA1 | RA2 | RA3 | RA4 | RA5 | RA6 | RA7 -> PORTA
  | RB0 | RB1 | RB2 | RB3 | RB4 | RB5 | RB6 | RB7 -> PORTB
  | RC0 | RC1 | RC2 | RC3 | RC4 | RC5 | RC6 | RC7 -> PORTC
  | RD0 | RD1 | RD2 | RD3 | RD4 | RD5 | RD6 | RD7 -> PORTD
  | RE0 | RE1 | RE2 | RE3 | RE4 | RE5 | RE6 | RE7 -> PORTE
  | RF0 | RF1 | RF2 | RF3 | RF4 | RF5 | RF6 | RF7 -> PORTF
  | RG0 | RG1 | RG2 | RG3 | RG4 | RG5 | RG6 | RG7 -> PORTG
  | RH0 | RH1 | RH2 | RH3 | RH4 | RH5 | RH6 | RH7 -> PORTH
  | RI0 | RI1 | RI2 | RI3 | RI4 | RI5 | RI6 | RI7 -> PORTI
  | RJ0 | RJ1 | RJ2 | RJ3 | RJ4 | RJ5 | RJ6 | RJ7 -> PORTJ
  | RK0 | RK1 | RK2 | RK3 | RK4 | RK5 | RK6 | RK7 -> PORTK
  | RL0 | RL1 | RL2 | RL3 | RL4 | RL5 | RL6 | RL7 -> PORTL
  | SPDR0 | SPDR1 | SPDR2 | SPDR3 | SPDR4 | SPDR5 | SPDR6 | SPDR7 -> SPDR

;;

let index_of_pin pin =
  match pin with
  | RA0 | RB0 | RC0 | RD0 | RE0 | RF0 | SPDR0 | RG0 | RH0 | RI0 | RJ0 | RK0 | RL0 -> 0
  | RA1 | RB1 | RC1 | RD1 | RE1 | RF1 | SPDR1 | RG1 | RH1 | RI1 | RJ1 | RK1 | RL1 -> 1
  | RA2 | RB2 | RC2 | RD2 | RE2 | RF2 | SPDR2 | RG2 | RH2 | RI2 | RJ2 | RK2 | RL2 -> 2
  | RA3 | RB3 | RC3 | RD3 | RE3 | RF3 | SPDR3 | RG3 | RH3 | RI3 | RJ3 | RK3 | RL3 -> 3
  | RA4 | RB4 | RC4 | RD4 | RE4 | RF4 | SPDR4 | RG4 | RH4 | RI4 | RJ4 | RK4 | RL4 -> 4
  | RA5 | RB5 | RC5 | RD5 | RE5 | RF5 | SPDR5 | RG5 | RH5 | RI5 | RJ5 | RK5 | RL5 -> 5
  | RA6 | RB6 | RC6 | RD6 | RE6 | RF6 | SPDR6 | RG6 | RH6 | RI6 | RJ6 | RK6 | RL6 -> 6
  | RA7 | RB7 | RC7 | RD7 | RE7 | RF7 | SPDR7 | RG7 | RH7 | RI7 | RJ7 | RK7 | RL7 -> 7



let pin_of_port_index port index =
  let error () = invalid_arg "Simul.pin_of_port_index" in
  match port with
    | PORTA ->
      begin match index with
        | 0 -> RA0 | 1 -> RA1 | 2 -> RA2 | 3 -> RA3 | 4 -> RA4 | 5 -> RA5
        | 6 -> RA6 | 7 -> RA7 | _ -> error ()
      end
    | PORTB ->
      begin match index with
        | 0 -> RB0 | 1 -> RB1 | 2 -> RB2 | 3 -> RB3 | 4 -> RB4 | 5 -> RB5
        | 6 -> RB6 | 7 -> RB7 | _ -> error ()
      end
    | PORTC ->
      begin match index with
        | 0 -> RC0 | 1 -> RC1 | 2 -> RC2 | 3 -> RC3 | 4 -> RC4 | 5 -> RC5
        | 6 -> RC6 | 7 -> RC7 | _ -> error ()
      end
    | PORTD ->
      begin match index with
        | 0 -> RD0 | 1 -> RD1 | 2 -> RD2 | 3 -> RD3 | 4 -> RD4 | 5 -> RD5
        | 6 -> RD6 | 7 -> RD7 | _ -> error ()
      end
    | PORTE ->
      begin match index with
        | 0 -> RE0 | 1 -> RE1 | 2 -> RE2 | 3 -> RE3 | 4 -> RE4 | 5 -> RE5
        | 6 -> RE6 | 7 -> RE7 | _ -> error ()
      end
    | PORTF ->
      begin match index with
        | 0 -> RF0 | 1 -> RF1 | 2 -> RF2 | 3 -> RF3 | 4 -> RF4 | 5 -> RF5
        | 6 -> RF6 | 7 -> RF7 | _ -> error ()
      end
        | PORTG ->
      begin match index with
        | 0 -> RG0 | 1 -> RG1 | 2 -> RG2 | 3 -> RG3 | 4 -> RG4 | 5 -> RG5
        | 6 -> RG6 | 7 -> RG7 | _ -> error ()
      end
        | PORTH ->
      begin match index with
        | 0 -> RH0 | 1 -> RH1 | 2 -> RH2 | 3 -> RH3 | 4 -> RH4 | 5 -> RH5
        | 6 -> RH6 | 7 -> RH7 | _ -> error ()
      end
        | PORTI ->
      begin match index with
        | 0 -> RI0 | 1 -> RI1 | 2 -> RI2 | 3 -> RI3 | 4 -> RI4 | 5 -> RI5
        | 6 -> RI6 | 7 -> RI7 | _ -> error ()
      end
        | PORTJ ->
      begin match index with
        | 0 -> RJ0 | 1 -> RJ1 | 2 -> RJ2 | 3 -> RJ3 | 4 -> RJ4 | 5 -> RJ5
        | 6 -> RJ6 | 7 -> RJ7 | _ -> error ()
      end
        | PORTK ->
      begin match index with
        | 0 -> RK0 | 1 -> RK1 | 2 -> RK2 | 3 -> RK3 | 4 -> RK4 | 5 -> RK5
        | 6 -> RK6 | 7 -> RK7 | _ -> error ()
      end
        | PORTL ->
      begin match index with
        | 0 -> RL0 | 1 -> RL1 | 2 -> RL2 | 3 -> RL3 | 4 -> RL4 | 5 -> RL5
        | 6 -> RL6 | 7 -> RL7 | _ -> error ()
      end
    | SPDR ->
      begin match index with
        | 0 -> SPDR0
        | 1 ->  SPDR1
        | 2 ->  SPDR2
        | 3 -> SPDR3
        | 4 -> SPDR4
        | 5 ->  SPDR5
        | 6 ->  SPDR6
        | 7 -> SPDR7
        | _ -> error ()
        end
;;

let pin_of_number s =
  match s with
  | "PIN0" -> RD2
  | "PIN1" -> RE1
  | "PIN2" -> RE4
  | "PIN3" -> RE5
  | "PIN4" -> RD4
  | "PIN5" -> RC6
  | "PIN6" -> RH3
  | "PIN7" -> RH4
  | "PIN8" -> RH5
  | "PIN9" -> RH6
  | "PIN10" -> RB4
  | "PIN11" -> RB5
  | "PIN12" -> RB6
  | "PIN13" -> RB7
  | "PIN18" -> RD3
  | "PIN19" -> RD2
  | "PIN20" -> RD1
  | "PIN21" -> RD0
  | "PINA0" -> RF0
  | "PINA1" -> RF1
  | "PINA2" -> RF2
  | "PINA3" -> RF3
  | "PINA4" -> RF4
  | "PINA5" -> RF5
  | _ -> raise (Invalid_argument ("Wrong pin "^s));;

let pin_of_string s =
  let error () = invalid_arg ("Simul.pin_of_string : "^s) in
  if String.length s <> 3 || (s.[0] <> 'R' && s.[0] <> 'r') then error ();
  try
    let port = port_of_char s.[1] in
    let index = int_of_char s.[2] - int_of_char '0' in
    pin_of_port_index port index
  with Invalid_argument _ -> error ()
;;

(***)

type an =
  | AN0  | AN1  | AN2  | AN3
  | AN4  | AN5  | AN6  | AN7
  | AN8  | AN9  | AN10 | AN11
  | AN12

let string_of_an an = match an with
  | AN0  -> "AN0"  | AN1 -> "AN1" | AN2  -> "AN2"  | AN3  -> "AN3"
  | AN4  -> "AN4"  | AN5 -> "AN5" | AN6  -> "AN6"  | AN7  -> "AN7"
  | AN8  -> "AN8"  | AN9 -> "AN9" | AN10 -> "AN10" | AN11 -> "AN11"
  | AN12 -> "AN12"

let an_of_string str = match str with
  | "AN0"  -> AN0  | "AN1" -> AN1 | "AN2"  -> AN2  | "AN3"  -> AN3
  | "AN4"  -> AN4  | "AN5" -> AN5 | "AN6"  -> AN6  | "AN7"  -> AN7
  | "AN8"  -> AN8  | "AN9" -> AN9 | "AN10" -> AN10 | "AN11" -> AN11
  | "AN12" -> AN12 | _ -> invalid_arg "Simul.an_of_string"

let char_of_an an = match an with
  | AN0  -> '0' | AN1 -> '1' | AN2  -> '2' | AN3  -> '3'
  | AN4  -> '4' | AN5 -> '5' | AN6  -> '6' | AN7  -> '7'
  | AN8  -> '8' | AN9 -> '9' | AN10 -> 'A' | AN11 -> 'B'
  | AN12 -> 'C'

let an_of_char c = match c with
  | '0' -> AN0  | '1' -> AN1 | '2' -> AN2  | '3' -> AN3
  | '4' -> AN4  | '5' -> AN5 | '6' -> AN6  | '7' -> AN7
  | '8' -> AN8  | '9' -> AN9 | 'A' -> AN10 | 'B' -> AN11
  | 'C' -> AN12 | _ -> invalid_arg "Simul.an_of_char"

let int_of_an an = match an with
  | AN0  -> 0x0 | AN1 -> 0x1 | AN2  -> 0x2 | AN3  -> 0x3
  | AN4  -> 0x4 | AN5 -> 0x5 | AN6  -> 0x6 | AN7  -> 0x7
  | AN8  -> 0x8 | AN9 -> 0x9 | AN10 -> 0xA | AN11 -> 0xB
  | AN12 -> 0xC

let an_of_int n = match n with
  | 0x0  -> AN0 | 0x1 -> AN1 | 0x2  -> AN2 | 0x3  -> AN3
  | 0x4  -> AN4 | 0x5 -> AN5 | 0x6  -> AN6 | 0x7  -> AN7
  | 0x8  -> AN8 | 0x9 -> AN9 | 0xA -> AN10 | 0xB -> AN11
  | 0xC -> AN12 | _ -> invalid_arg "Simul.an_of_int"

(***)

type input = IWrite of port * int | ITris of port * int | IWriteAnalog of an * int | IConfigAnalog of int | ISync | IStop

let input_of_string s =
  let error () = invalid_arg ("Simul.input_of_string : "^s) in
  match s with
    | "SYNC" -> ISync
    | "STOP" -> IStop
    | _ ->
      try
        match s.[0] with
        | 'W' -> assert (String.length s = 4); IWrite  (port_of_char s.[1], int_of_hex2 s.[2] s.[3])
        | 'T' -> assert (String.length s = 4); ITris   (port_of_char s.[1], int_of_hex2 s.[2] s.[3])
        | 'Z' -> assert (String.length s = 5); IWriteAnalog (an_of_char s.[1], int_of_hex3 s.[2] s.[3] s.[4])
        | 'C' -> assert (String.length s = 2); IConfigAnalog (int_of_hex1 s.[1])
        | _ -> error ()
      with _ -> error ()
;;

(***)

type output = OSet of pin | OClear of pin | OWrite of port * int | OWriteAnalog of an * int | ODone | OStop

let string_of_output output =
  match output with
    | OSet pin ->
      Printf.sprintf "S%c%d" (char_of_port (port_of_pin pin))
        (index_of_pin pin)
    | OClear pin ->
      Printf.sprintf "C%c%d" (char_of_port (port_of_pin pin))
        (index_of_pin pin)
    | OWrite (port, value) ->
      if value < 0 || value > 0xFF
      then failwith (Printf.sprintf "value %d of OWrite is out of range [ 0x0; 0xFF ]" value);
      Printf.sprintf "W%c%02X" (char_of_port port) value
    | OWriteAnalog (an, value) ->
      if value < 0 || value > 0x3FF
      then failwith (Printf.sprintf "value %d of OWriteAnalog is out of range [ 0x0; 0x3FF ]" value);
      Printf.sprintf "Z%c%03X" (char_of_an an) value
    | ODone -> "DONE"
    | OStop -> "STOP"
;;

(***)

let channel = Event.new_channel ();;

let rec send_loop () =
  begin try
          let output = (Event.sync (Event.receive channel)) in
          print_string (string_of_output output);
          print_char '\n';
          flush stdout;
    with exn ->
      Printf.eprintf "Unhandled exception %s\n%!" (Printexc.to_string exn)
  end;
  send_loop ();
in
ignore (Thread.create send_loop ());
;;

let send output = Event.sync (Event.send channel output);;

(***)

type handler =
  | Exit_handler of (unit -> unit)
  | Write_handler of (port -> int -> unit)
  | Write_port_handler of port * (int -> unit)
  | Tris_handler of (port -> int -> unit)
  | Tris_port_handler of port * (int -> unit)
  | Set_handler of (pin -> unit)
  | Clear_handler of (pin -> unit)
  | Change_handler of (pin -> bool -> unit)
  | Set_pin_handler of pin * (unit -> unit)
  | Clear_pin_handler of pin * (unit -> unit)
  | Change_pin_handler of pin * (bool -> unit)
  | Setin_handler of (pin -> unit)
  | Setout_handler of (pin -> unit)
  | Setstate_handler of (pin -> bool -> unit)
  | Setin_pin_handler of pin * (unit -> unit)
  | Setout_pin_handler of pin * (unit -> unit)
  | Setstate_pin_handler of pin * (bool -> unit)
  | Write_analog_handler of (an -> int -> unit)
  | Write_an_analog_handler of an * (int -> unit)
  | Config_analogs_handler of (int -> unit)

let handlers_mutex = Mutex.create ();;
let handlers = ref [];;

let add_handler handler =
  Mutex.lock handlers_mutex;
  handlers := handler :: !handlers;
  Mutex.unlock handlers_mutex;
;;

let remove_handler handler =
  Mutex.lock handlers_mutex;
  handlers := List.filter ((!=) handler) !handlers;
  Mutex.unlock handlers_mutex;

;;

(***)

let ports = Array.make 13 0;; (* we count SPDR as a port *)
let triss = Array.make 13 0xFF;;
let analogs = Array.make 13 0;;
let analog_cnt = ref 0;;

let scall1 f arg =
  try Printexc.print f arg with _ -> ();
;;

let scall2 f arg1 arg2 =
  try Printexc.print (Printexc.print f arg1) arg2 with _ -> ();
;;

let exec input =
  match input with
  | IWrite (port, new_value) ->
     (* let s = Printf.sprintf "port %s = %d" (string_of_port port) new_value in *)
     (* if true then failwith s; *)
      assert (new_value >= 0 && new_value <= 255);
      let index = index_of_port port in
      let old_value = ports.(index) in
      let lxor_values = old_value lxor new_value in
      let set_pins = ref [] in
      let clear_pins = ref [] in
      let () =
        for i = 0 to 7 do
          let mask = 1 lsl i in
          if lxor_values land mask <> 0 then
            if old_value land mask <> 0 then
              clear_pins := pin_of_port_index port i :: !clear_pins
            else
              set_pins := pin_of_port_index port i :: !set_pins
        done;
      in
      let set_pins = !set_pins in
      let clear_pins = !clear_pins in
      let call h =
        match h with
          | Write_handler f -> scall2 f port new_value;
          | Write_port_handler (p, f) -> if p = port then scall1 f new_value;
          | Set_handler f -> List.iter (scall1 f) set_pins;
          | Clear_handler f -> List.iter (scall1 f) clear_pins;
          | Change_handler f ->
            List.iter (fun p -> scall2 f p true) set_pins;
            List.iter (fun p -> scall2 f p false) clear_pins;
          | Set_pin_handler (p, f) ->
            List.iter (fun q -> if p = q then scall1 f ()) set_pins;
          | Clear_pin_handler (p, f) ->
            List.iter (fun q -> if p = q then scall1 f ()) clear_pins;
          | Change_pin_handler (p, f) ->
            List.iter (fun q -> if p = q then scall1 f true) set_pins;
            List.iter (fun q -> if p = q then scall1 f false) clear_pins;
          | _ -> ()
      in
      ports.(index) <- new_value;
      List.iter call !handlers;
      true
    | ITris (port, new_value) ->
      assert (new_value >= 0 && new_value <= 0xFF);
      let index = index_of_port port in
      let old_value = ports.(index) in
      let lxor_values = old_value lxor new_value in
      let new_in_pins = ref [] in
      let new_out_pins = ref [] in
      let () =
        for i = 0 to 7 do
          let mask = 1 lsl i in
          if lxor_values land mask <> 0 then
            if old_value land mask <> 0 then
              new_out_pins := pin_of_port_index port i :: !new_out_pins
            else
              new_in_pins := pin_of_port_index port i :: !new_in_pins
        done;
      in
      let new_in_pins = !new_in_pins in
      let new_out_pins = !new_out_pins in
      let call h =
        match h with
          | Tris_handler f -> scall2 f port new_value;
          | Tris_port_handler (p, f) -> if p = port then scall1 f new_value;
          | Setin_handler f -> List.iter (scall1 f) new_in_pins;
          | Setout_handler f -> List.iter (scall1 f) new_out_pins;
          | Setstate_handler f ->
            List.iter (fun p -> scall2 f p true) new_in_pins;
            List.iter (fun p -> scall2 f p false) new_out_pins;
          | Setin_pin_handler (p, f) ->
            List.iter (fun q -> if p = q then scall1 f ()) new_in_pins;
          | Setout_pin_handler (p, f) ->
            List.iter (fun q -> if p = q then scall1 f ()) new_out_pins;
          | Setstate_pin_handler (p, f) ->
            List.iter (fun q -> if p = q then scall1 f true) new_in_pins;
            List.iter (fun q -> if p = q then scall1 f false) new_out_pins;
          | _ -> ()
      in
      triss.(index) <- new_value;
      List.iter call !handlers;
      true
    | IWriteAnalog (an, value) ->
      assert (value >= 0 && value <= 0x3FF);
      analogs.(int_of_an an) <- value;
      List.iter (fun handler -> match handler with
        | Write_analog_handler f -> f an value
        | Write_an_analog_handler (an', f) when an' = an -> f value
        | _ -> ()
      ) !handlers;
      true
    | IConfigAnalog cnt ->
      assert (cnt >= 0 && cnt <= 13);
      analog_cnt := cnt;
      List.iter (fun handler -> match handler with
        | Config_analogs_handler f -> f cnt
        | _ -> ()
      ) !handlers;
      true
    | ISync -> send ODone; true
    | IStop ->
      let call h = match h with Exit_handler f -> scall1 f () | _ -> () in
      List.iter call !handlers;
      false
;;

(***)

let (start, join) =
  let rec receive_loop () =
    let s = read_line () in
    match
      try Some (input_of_string s)
      with Invalid_argument _ ->
        Printf.eprintf "Invalid instruction: `%s'\n%!" s;
        None
    with
      | Some input -> if exec input then receive_loop ();
      | None -> receive_loop ();
  in
  let loop_thread_mutex = Mutex.create () in
  let loop_thread = ref None in
  let start () =
    Mutex.lock loop_thread_mutex;
    match !loop_thread with
      | Some _ ->
        Mutex.unlock loop_thread_mutex;
        failwith "invalid call to Simul.start, simulator already running";
      | None ->
        loop_thread := Some (Thread.create receive_loop ());
        Mutex.unlock loop_thread_mutex;
  and join () =
    Mutex.lock loop_thread_mutex;
    match !loop_thread with
      | None ->
        Mutex.unlock loop_thread_mutex;
        failwith "invalid call to Simul.join, simulator is not running";
      | Some th ->
        Mutex.unlock loop_thread_mutex;
        Thread.join th;
  in
  (start, join)
;;

(***)

let write_port port value =
  send (OWrite (port, value mod 256));;

let set_pin pin = send (OSet pin);;

let clear_pin pin = send (OClear pin);;

let change_pin pin b = send (if b then OSet pin else OClear pin);;

let write_analog an value = send (OWriteAnalog (an, value));;

let read_port port = ports.(index_of_port port);;

let read_tris port = triss.(index_of_port port);;

let test_pin pin =
  try
    let value = ports.(index_of_port (port_of_pin pin)) in
    let mask = 1 lsl (index_of_pin pin) in
    (value land mask) <> 0
  with _ -> invalid_arg "test_pin"
;;

let state_pin pin =
  try
    let value = triss.(index_of_port (port_of_pin pin)) in
    let mask = 1 lsl (index_of_pin pin) in
    (value land mask) <> 0
  with _ -> invalid_arg "state_pin"
;;

let analog_input_count () =
  !analog_cnt
;;
