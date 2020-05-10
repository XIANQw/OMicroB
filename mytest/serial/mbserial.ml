let _ =
  Serial.init ();
  while(true) do
    Serial.write ("Hello world\n");
    delay 2000;
  done