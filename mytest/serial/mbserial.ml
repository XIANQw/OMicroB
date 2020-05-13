Open Screen
let _ =
  print_int 42;
  Serial.init ();
  while(true) do
    Serial.write ("Hello world\n");
    Screen.print_string (Serial.read ()); (* Weirdly this takes ~1s *)
    delay 200;
  done