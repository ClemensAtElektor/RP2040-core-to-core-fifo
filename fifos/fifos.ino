/*
 * Core-to-core communication using the FIFOs inside the SIO block.
 * 
 * Arduino IDE for RP2040: d:\dev\raspberry-pi\rp2040\arduino-1.8.15
 */

void core_debug(char *p_msg)
{
  Serial.printf("core%d - %s",get_core_num(),p_msg);
}

void core_print_uint32(uint32_t data)
{
  Serial.printf("core%d - 0x%08x (%d)\n",get_core_num(),data,data);
}

bool write_to_other_core(uint32_t data)
{
  if (rp2040.fifo.push_nb(data)==false)
  {
    core_debug((char*)"failed to push\n");
    return false;
  }
  return true;
}

bool read_from_other_core(uint32_t *p_data)
{
  if (rp2040.fifo.available())
  {
    if (rp2040.fifo.pop_nb(p_data)==false)
    {
      core_debug((char*)"failed to pop\n");
      return false;
    }
    return true;
  }
  return false;
}

// Core 0
void setup(void) 
{
}

// Core 0
void loop(void) 
{
  uint32_t message = 0;
  if (read_from_other_core(&message)==true)
  {
    // Do something useful with message.
    core_print_uint32(message);
  }

  write_to_other_core(millis());
  
  delay(1000);
}

// Core 1
void setup1(void)
{
}

// Core 1
void loop1(void)
{
  uint32_t message = 0;
  if (read_from_other_core(&message)==true)
  {
    // Do something useful with message.
    core_print_uint32(message);
  }

  write_to_other_core(millis());
  
  delay(1100);
}
