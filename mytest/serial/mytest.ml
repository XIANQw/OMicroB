let _ =
  Serial.init ();
  while(true) do
    Serial.write ("H");
    delay 200;
  done
