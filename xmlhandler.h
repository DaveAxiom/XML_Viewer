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

enum Node_state
{
	ns_empty,
	ns_tag,
	ns_text
};

struct DisplayInfo
{
	CString m_text;
	CString m_closing_tag;

	bool m_bclosing;
	Node_state m_node_state;
	int m_change_indent;
};


class XMLNode
{
friend class XMLHandler;
public:
	XMLNode();
	~XMLNode();

private:
	//int node_state;
	Node_state m_node_state;
	CString m_node_text_or_tag_name;
	CPtrArray m_nodes;
};

class XMLHandler
{
friend class XMLNode;
public:
	void add_xml_text(CString&);
	void Display(bool);
	DisplayInfo DisplayNext();
	DisplayInfo DisplayNextTextExport();

	int Search(CString&);
	bool enter_node(CString&, int);
	void exit_node();
	CString get_node_info(CString&);
	int get_node_int(CString&);
	CTime get_node_date(CString&);
	void done_adding_text();
	void reset();
	void add_mode();
	void add_tag(CString, bool);
	void add_to_text_node(CString&);

private:
	CPtrArray m_node_history;
	XMLNode* m_root_xml_node;
	CString m_potential_tag_name;
	CString m_leftover_text;
	CString find_tagname(CString&, int);

	bool m_looking_for_end_arrow;
	bool m_looking_for_end_arrow_to_terminating_tag;

	XMLNode* RetrieveNextNode(bool);
	XMLNode* RetrieveNextSiblingNode(bool);

	enum HandlerStatus
	{
		hs_none,
		hs_adding,
		hs_searching,
		hs_displaying
	} m_status;

public:
	XMLHandler();
	~XMLHandler();
};
