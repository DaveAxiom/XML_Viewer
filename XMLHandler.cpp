/*
XML Viewer
Copyright 2017 David V. Makray

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this 
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation 
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors 
may be used to endorse or promote products derived from this software without 
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// XMLHandler.cpp : implementation of the XMLNode, XMLHandler classes
//

#include "stdafx.h"
#include "XMLHandler.h"

XMLNode::XMLNode()
{
	m_node_state = ns_empty;
	m_node_text_or_tag_name.Empty();
	m_nodes.SetSize(0, 2);
}

XMLNode::~XMLNode()
{
	m_node_state = ns_empty;
	m_node_text_or_tag_name.Empty();
	
	XMLNode* temp;
	for (int arrayloop = 0; arrayloop < m_nodes.GetSize(); arrayloop++)
	{
		temp = (XMLNode*)m_nodes[arrayloop];

		if (temp != NULL)
		{
			delete temp;
			temp = NULL;
		}
	}
}

XMLHandler::XMLHandler()
{
	m_root_xml_node = new XMLNode();
	
	//Set a value of a tag that can't be found for now..
	m_root_xml_node->m_node_text_or_tag_name = "whole_doc";
	m_node_history.SetSize(0, 2);
	m_status = hs_none;
	m_leftover_text.Empty();
}

XMLHandler::~XMLHandler()
{
	delete m_root_xml_node;
	m_node_history.RemoveAll();
}

void XMLHandler::add_xml_text(CString& in_text)
{
	if (m_status != hs_adding)
	{
		if (m_status == hs_none)
		{
			m_node_history.Add((void*)m_root_xml_node);
			m_status = hs_adding;

			m_looking_for_end_arrow = false;
			m_looking_for_end_arrow_to_terminating_tag = false;
		}
		else
		{
			//Searching or display may have to finish first.
			return;
		}
	}
	
	//Take the leftover text and put it back into the parser.
	in_text = m_leftover_text + in_text;
	
	if (!m_looking_for_end_arrow)
	{
		in_text+= " ";
	}

	m_leftover_text.Empty();
	CString tag_name_pending_end_arrow;
	for (int loop = 0; loop < in_text.GetLength(); loop++)
	{
		if (in_text[loop] == '<')
		{

			if (loop > 0)
			{
				TRACE("The text to take is: %s\n", in_text.Left(loop));
				//Take the text.

				add_to_text_node(in_text.Left(loop));
				in_text = in_text.Right(in_text.GetLength() - loop);
				loop = 0;
			}

			m_looking_for_end_arrow = true;

			//Looks like the previous [if any] is an incomplete terminating tag.
			m_looking_for_end_arrow_to_terminating_tag = false;

			//Check to see if this is an ending tag..
			ASSERT(in_text.GetLength() >= loop + 1);
			if (in_text[loop + 1] == '/')
			{
				//Get the name of this tag.
				m_potential_tag_name = find_tagname(in_text, loop + 2);
				m_looking_for_end_arrow_to_terminating_tag = true;
			}
			else
			{
				m_potential_tag_name = find_tagname(in_text, loop + 1);
			}
		}

		if (in_text[loop] == '>' && m_looking_for_end_arrow)
		{
			//Found a complete tag.. Clear out the tag text.
			//Check to see if it terminates itself.. like "/>"
			//If so don't bother adding it to history and "continue;".
			if (loop > 0)
			{
				if (in_text[loop - 1] == '/')
				{
					//looking_for_end_arrow_to_terminating_tag = true;
					XMLNode* self_terminate_node = new XMLNode();
					self_terminate_node->m_node_text_or_tag_name = m_potential_tag_name;
					self_terminate_node->m_node_state = ns_tag;
					((XMLNode*)m_node_history[m_node_history.GetUpperBound()])->m_nodes.Add((void*)self_terminate_node);
					m_looking_for_end_arrow = false;
					m_looking_for_end_arrow_to_terminating_tag = false;
				}
			}

			//Check to see if it finishes a terminating tag from bool."</"
			if (m_looking_for_end_arrow_to_terminating_tag)
			{
				m_looking_for_end_arrow_to_terminating_tag = false;

				//Look to see if this latest match is the latest in history.
				if (((XMLNode*)m_node_history[m_node_history.GetUpperBound()])->m_node_text_or_tag_name == m_potential_tag_name)
				{
					//It also must be a tag type to remove. (Hint:not empty). If so remove it.
					m_node_history.RemoveAt(m_node_history.GetUpperBound());
				}
#ifdef _DEBUG
				else
				{
					TRACE("Ending tag didn't match!\n");
				}
#endif
			}
			else if (m_looking_for_end_arrow)
			{
				//Add it to the tree node.
				XMLNode* temp_node = new XMLNode();
				temp_node->m_node_text_or_tag_name = m_potential_tag_name;
				temp_node->m_node_state = ns_tag;
				
				//Put into the node on the current furthest branch.
				((XMLNode*)m_node_history[m_node_history.GetUpperBound()])->m_nodes.Add((void*)temp_node);

				//Plus add it to history.
				m_node_history.Add((void*)temp_node);

			}

			in_text = in_text.Right(in_text.GetLength() - loop - 1);
			m_looking_for_end_arrow = false;
			loop = -1;
		}
	}

	//Take care of leftover text
	m_leftover_text += in_text;
	in_text.Empty();
}

void XMLHandler::add_to_text_node(CString& text)
{
	//Find out if the last node is a text node..
	XMLNode* last_node = (XMLNode*)m_node_history[m_node_history.GetUpperBound()];
	XMLNode* ptemp_node = NULL;

	if (text == " ")
	{
		text.Empty();
		return;
	}

	text.TrimLeft();
	
	int white_space = 0;
	for (int clip_loop = 0; clip_loop < text.GetLength(); clip_loop++)
	{
		if (text[clip_loop] == ' ')
		{
			white_space++;
		}
		else
		{
			if(white_space > 1)
			{
				text = text.Left(clip_loop - white_space + 1) + text.Right(text.GetLength() - clip_loop);
				clip_loop -= white_space - 1;
			}
			white_space = 0;
		}
	}
	
	text.TrimRight();
	if (text.IsEmpty())
	{
		return;
	}

	text += " ";

	//Rule: Last node is always a tag while adding
	if (last_node->m_nodes.GetSize() > 0)
	{
		XMLNode* text_check = ((XMLNode*)(last_node->m_nodes[last_node->m_nodes.GetUpperBound()]));
		if (text_check->m_node_state != ns_text)
		{
			//Create a new text node..
			ptemp_node = new XMLNode();
			ASSERT(ptemp_node);

			last_node->m_nodes.Add((void*)ptemp_node);
		}
		else
		{
			ptemp_node = text_check;
		}
	}
	
	//Check to see if there's no text node to append too.
	if (last_node->m_nodes.GetSize() == 0)
	{
		ASSERT(last_node->m_node_state != ns_text);
		//Create a new text node..
		ptemp_node = new XMLNode();
				
		ASSERT(ptemp_node);
		last_node->m_nodes.Add((void*)ptemp_node);
	}

	//Set the text of this node...
	//Removal of extra spaces etc too be taken care of here.
	if (!ptemp_node->m_node_text_or_tag_name.IsEmpty())
	{
		ptemp_node->m_node_text_or_tag_name += " ";
	}
	ptemp_node->m_node_text_or_tag_name += text;
	ptemp_node->m_node_state = ns_text;
}

CString XMLHandler::find_tagname(CString& in, int start_pos)
{
	int name_check_loop;
	for (name_check_loop = start_pos; name_check_loop < in.GetLength(); name_check_loop++)
	{
		//This could be a while instead.
		if (in[name_check_loop] == 32 || in[name_check_loop] == 10 || in[name_check_loop] == 13 || in[name_check_loop] == '<' || in[name_check_loop] == '>' || in[name_check_loop] == '/')
		{
			//Looks like the name has been found..
			break;
		}
	}
	return(in.Mid(start_pos, name_check_loop - start_pos));

}

void XMLHandler::Display(bool start_is_true)
{
	if (start_is_true)
	{
		if (m_status != hs_none)
		{
			m_node_history.RemoveAll();
		}
		m_status = hs_displaying;
		m_node_history.Add((void*)m_root_xml_node);
	}
	else
	{
		m_node_history.RemoveAll();
		m_status = hs_none;
	}

}

DisplayInfo XMLHandler::DisplayNext()
{
	DisplayInfo di;
	di.m_change_indent = 0;
	di.m_closing_tag = "";
	//The m_node_history keeps track of this procedure.
	
	//Look at the latest node in history..
	XMLNode* latest_in_history = ((XMLNode*)m_node_history[m_node_history.GetUpperBound()]);
	if (latest_in_history->m_node_state != ns_text)
	{
		//Does this node have sub-notes?
		if (latest_in_history->m_nodes.GetSize() > 0)
		{
			//Display the first node.
			XMLNode* node_to_display = ((XMLNode*)(latest_in_history->m_nodes[0]));

			di.m_text = node_to_display->m_node_text_or_tag_name;
			di.m_node_state = node_to_display->m_node_state;

			if (m_node_history.GetSize() != 1)
			{
				di.m_change_indent++;
			}
			
			m_node_history.Add((void*)node_to_display);
			return(di);
		}
	}

	//Move on in the tree and find the next node to display.
	XMLNode* checking_for_sub_nodes = latest_in_history;
	while(checking_for_sub_nodes->m_node_state != ns_empty)
	{
		//Make sure the last one is saved:
		latest_in_history = ((XMLNode*)m_node_history[m_node_history.GetUpperBound()]);

		//Remove the last one from history..
		int total_in_history = m_node_history.GetUpperBound();
		m_node_history.RemoveAt(total_in_history);
		
		if (latest_in_history->m_node_state == ns_tag)
		{
			di.m_closing_tag = latest_in_history->m_node_text_or_tag_name;
		}

		checking_for_sub_nodes = ((XMLNode*)m_node_history[total_in_history - 1]);
		  
		for (int search_loop = 0; search_loop < checking_for_sub_nodes->m_nodes.GetUpperBound(); search_loop++)
		{
			if (((XMLNode*)(checking_for_sub_nodes->m_nodes[search_loop])) == latest_in_history)
			{
				//The next one is returned.
				XMLNode* node_to_display = ((XMLNode*)(checking_for_sub_nodes->m_nodes[search_loop + 1]));
				di.m_text = node_to_display->m_node_text_or_tag_name;
				di.m_node_state = node_to_display->m_node_state;
				m_node_history.Add((void*)node_to_display);
				return(di);
			}
		}
		di.m_change_indent--;
	}

	di.m_text = "";
	di.m_node_state = ns_empty;
	return(di);
}


DisplayInfo XMLHandler::DisplayNextTextExport()
{
	static bool travel_deeper = true;

	/*Sending the closing tag becomes true when, 
	1. A tag has no children
	2. A tag's contents has begun to be processed, being either a sub tag or a text node.
	3. The following function call must send a closing tag.
	
	It reads false when.
	1. A sub tag needs to sent out.*/

	DisplayInfo di;
	di.m_change_indent = 0;
	di.m_bclosing = false;

	//The m_node_history is going to keep track of this whole proccess.
	
	//Look at the latest node in history..
	XMLNode* latest_in_history = ((XMLNode*)m_node_history[m_node_history.GetUpperBound()]);
	XMLNode* current_worked_node = RetrieveNextNode(travel_deeper);

	TRACE("%s, ", latest_in_history->m_node_text_or_tag_name);
	TRACE("%s\n", current_worked_node->m_node_text_or_tag_name);
	if (current_worked_node->m_node_state == ns_empty /*&& !travel_deeper*/)
	{
		//Done with
		di.m_text == "";
		di.m_node_state = ns_empty;
		return(di);
	}

	//A tag was found...
	if(current_worked_node->m_node_state != ns_text)
	{
		if (travel_deeper)
		{//Start tag.
			di.m_text = current_worked_node->m_node_text_or_tag_name;
			di.m_node_state = current_worked_node->m_node_state;

			//What if this were a self terminating tag?
			//Found a tag, move in deeper, check otherwise.
			di.m_text = "<" + current_worked_node->m_node_text_or_tag_name + ">";
			m_node_history.Add((void*)current_worked_node);
			return(di);
		}
		else
		{//End tag.
			di.m_text = "</" + current_worked_node->m_node_text_or_tag_name;
			di.m_node_state = current_worked_node->m_node_state;

			TRACE("Figure this one out.\n");
			travel_deeper = false;	//Rise or stay at same level.
			//Now working only with an end tag..!!
		}
	}

	if(current_worked_node->m_node_state == ns_text)
	{
		//Check if latest is the last text node with end tag needing to be sent next call.
		//This text node may have siblings. Look just in case.
		travel_deeper = true; //Stay at the same level or rise.
		m_node_history.Add((void*)current_worked_node);

		//Send the text.
		XMLNode* next_call_node = RetrieveNextNode(true);
		return(di);
	}
	di.m_text = "";
	return(di);
}

XMLNode* XMLHandler::RetrieveNextNode(bool travel_deeper)
{
	/*The retrieval of preceding nodes includes opening and closing tags.*/
	//Travel deeper helps determine if the current node had been discovered, and shouldn't be re-traversed.
	XMLNode* latest_in_history = ((XMLNode*)m_node_history[m_node_history.GetUpperBound()]);
	if (latest_in_history->m_nodes.GetSize() > 0 && travel_deeper)	//Does it have children?
	{
		return(((XMLNode*)(latest_in_history->m_nodes[0])));
	}

	ASSERT(m_node_history.GetSize() > 1);
	XMLNode* parent_of_latest = ((XMLNode*)m_node_history[m_node_history.GetUpperBound() - 1]);
	if (parent_of_latest->m_node_state == ns_empty)
	{
		return(parent_of_latest);
	}
	for (int search_loop = 0; search_loop < parent_of_latest->m_nodes.GetUpperBound(); search_loop++)
	{
		if (((XMLNode*)(parent_of_latest->m_nodes[search_loop])) == latest_in_history)
		{
			if (parent_of_latest->m_nodes.GetUpperBound() > search_loop)
			{
				return((XMLNode*)parent_of_latest->m_nodes[search_loop + 1]);
			}
			else
			{
				return(parent_of_latest);
			}
		}
	}
	return(NULL);
}

XMLNode* XMLHandler::RetrieveNextSiblingNode(bool isfirst)
{

	XMLNode* current_node = ((XMLNode*)m_node_history[m_node_history.GetUpperBound()]);
	XMLNode* parent_node = ((XMLNode*)m_node_history[m_node_history.GetUpperBound() - 1]);
	if (isfirst)	//Looking for the first sibling.
	{
		parent_node = current_node;
		if (parent_node->m_nodes.GetSize() == 0)
		{
			return(NULL);
		}
		return((XMLNode*)parent_node->m_nodes[0]);
	}

	for (int search_loop = 0; search_loop < parent_node->m_nodes.GetUpperBound(); search_loop++)
	{
		if (((XMLNode*)(parent_node->m_nodes[search_loop])) == current_node)	//If current is found.
		{
			if (parent_node->m_nodes.GetUpperBound() > search_loop)
			{
				return((XMLNode*)parent_node->m_nodes[search_loop + 1]);
			}
			else
			{
				break;
			}
		}
	}
	return(NULL);
}

//Search() counts the amount of hits on a tag name or text of a node.
int XMLHandler::Search(CString& search_text)
{
	if (m_status != hs_none && m_status != hs_searching && m_status != hs_adding)
	{
		return(-1);
	}

	if (m_status == hs_none)
	{
		m_node_history.Add((void*)m_root_xml_node);
		m_status = hs_searching;
	}

	int result = 0;
	XMLNode* node_to_search = (XMLNode*)m_node_history[m_node_history.GetUpperBound()];
	
	for (int search = 0; search < node_to_search->m_nodes.GetSize(); search++)
	{
		if (((XMLNode*)node_to_search->m_nodes[search])->m_node_text_or_tag_name == search_text)
		{
			result++;
		}
	}
	return(result);
}

void XMLHandler::done_adding_text()
{
	m_status = hs_none;
	m_node_history.RemoveAll();
}

bool XMLHandler::enter_node(CString& node_name, int which)
{
	//Done only in search mode.
	if (m_status != hs_searching && m_status != hs_adding)
	{
		return(false);
	}
	
	if (Search(node_name) > which)
	{
		//Enter node here.
		XMLNode* node_to_search = (XMLNode*)m_node_history[m_node_history.GetUpperBound()];
	
		int count = 0;
		for (int name_search = 0; name_search < node_to_search->m_nodes.GetSize(); name_search++)
		{
			if (((XMLNode*)node_to_search->m_nodes[name_search])->m_node_text_or_tag_name == node_name)
			{
				if (count == which)
				{
					m_node_history.Add((void*)node_to_search->m_nodes[name_search]);
					break;
				}
				count++;
			}
		}
	}
	else
	{
		return(false);
	}
	return(true);
}

//exit_node() will back out from a node while in searching or adding mode.
void XMLHandler::exit_node()
{
	if (m_status != hs_searching && m_status != hs_adding)
	{
		return;
	}

	if (m_node_history.GetUpperBound() > 0)
	{
		m_node_history.RemoveAt(m_node_history.GetUpperBound());
	}
}

CString XMLHandler::get_node_info(CString& node_name)
{
	if (Search(node_name))
	{
		if (enter_node(node_name, 0))
		{
			XMLNode* temp = (XMLNode*)m_node_history[m_node_history.GetUpperBound()];
			if (temp->m_nodes.GetUpperBound() != -1)
			{
				exit_node();
				//Will double check later for text node.
				return(((XMLNode*)temp->m_nodes[0])->m_node_text_or_tag_name);
			}
			exit_node();
		}
	}
	return(CString(""));
}

int XMLHandler::get_node_int(CString& node_name)
{
	CString info = get_node_info(node_name);
	char* to_convert = info.GetBuffer(0);
	return(atoi(to_convert));
}

CTime XMLHandler::get_node_date(CString& node_name)
{
	CString info = get_node_info(node_name);
	TRACE("Node date: '%s' info found\n", info);
	int month = atoi(info.GetBuffer(0));
	if (month < 1 || month > 12 || info.GetLength() < 6)
	{
		return(CTime::GetCurrentTime());	//Bail on invalid data
	}
	int day, skip;
	if (month < 10)
	{
		skip = 2;
	}
	else
	{
		skip = 3;
	}
	day = atoi(info.GetBuffer(0) + skip);
	if (day < 1 || (day > 31 && day < 1900))
	{
		return(CTime::GetCurrentTime());	//Bail on invalid data
	}
	int year;
	if (day > 1900)
	{
		year = day;
		day = 1;	//0 or assign it 1.
	}
	else
	{
		if (day < 10)
		{
			skip += 2;
		}
		else
		{
			skip += 3;
		}
		year = atoi(info.GetBuffer(0) + skip);
	}
	return(CTime(year, month, day, 0, 0, 0));
}

void XMLHandler::reset()
{
	m_status = hs_none;
	m_node_history.RemoveAll();
}

void XMLHandler::add_mode()
{
	if (m_status == hs_none)
	{
		m_status = hs_adding;
		m_node_history.RemoveAll();
		m_node_history.Add((void*)m_root_xml_node);
	}
}

void XMLHandler::add_tag(CString tag_name, bool enter_tag)
{
	//Until a better method is possible.
	if (m_status == hs_none)
	{
			m_node_history.Add((void*)m_root_xml_node);
			m_status = hs_adding;
	}

	ASSERT(m_status == hs_adding);
	if (m_status != hs_adding)
	{
		return;
	}

	XMLNode* new_tag = new XMLNode();
	new_tag->m_node_text_or_tag_name = tag_name;
	new_tag->m_node_state = ns_tag;

	ASSERT(m_node_history.GetUpperBound() >= 0);
	((XMLNode*)m_node_history[m_node_history.GetUpperBound()])->m_nodes.Add((void*)new_tag);
	if (enter_tag)
	{
		m_node_history.Add((void*)new_tag);
	}
}