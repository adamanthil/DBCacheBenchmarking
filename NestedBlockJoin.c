

bool NBJ::moveNext()
{
  if (inner_block.is_empty() == false)
    {
  m_buffer.reset();
  while (m_buffer.full()== false && m_outer.moveNext())
    {
      MemoryBlock b = m_outer.next();
      foreach (Tuple t in inner_block)
	{

	  foreach (Tuple t0 in b)
	    {
	      if (match(t, t0))
		{
		  m_buffer.add(t, t0, attributes);
		}
	    }
	}
    }

	if (m_outer.is_eof())
	  {
	    m_inner_block = m_inner.next();
	  }
    }
}
