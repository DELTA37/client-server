#include "Server.hpp"



//-------------------Detalization--------------------------


void CServer::MainLoop(void)
{
		while(1)
		{
				fd_set s_read;
				fd_set s_err;

				memcpy(&s_read, &(this->s_client), (int)sizeof(fd_set));
				memcpy(&s_err, &(this->s_client), (int)sizeof(fd_set));


				struct timeval timeout;
				timeout.tv_sec = 3;
				timeout.tv_usec = 0;
				

				int act = select(FD_SETSIZE, &s_read, NULL, &s_err, &timeout);
				if (act < 0)
				{
						std::cout << "error in select" << std::endl;
						return;
				}
				if (act > 0)
				{

						for (int i = 0; i < this->count; i++)
						{
								if (FD_ISSET(client[i], &s_err))
								{
										RemoveClient(i);
								}
						}

						if (FD_ISSET(this->sockfd, &s_read))
						{
								this->AddClient();
						}

						for (int i = 0; i < this->count; i++)
						{
								if (FD_ISSET(client[i], &s_read))
								{
#ifdef DEBUG
										std::cout << "Ready read client of number: " << client[i] << std::endl;
#endif
										std::string msg = this->ReadFrom(i);
										std::cout << "MainLoop print: " << msg << std::endl;
												
										this->Do(i, msg);
								}
						}
				}
		}
		return;
}



bool CServer::IsReadyRead(int i)
{
		fd_set fds;
		struct timeval tv;

		FD_ZERO(&fds);

		FD_SET(this->client[i], &fds);

		tv.tv_sec = tv.tv_usec = 0;

		int rc = select(this->client[i] + 1, &fds, NULL, NULL, &tv);

		if (rc < 0)
		{
				return -1;
		}

		return FD_ISSET(this->client[i], &fds) ? 1 : 0;
}

bool CServer::IsReadyWrite(int i)
{
		fd_set fds;
		struct timeval tv;

		FD_ZERO(&fds);

		FD_SET(this->client[i], &fds);

		tv.tv_sec = tv.tv_usec = 3;

		int rc = select(this->client[i] + 1, NULL, &fds, NULL, &tv);

		if (rc < 0)
		{
				return -1;
				std::cout << "Select error";
		}

		return FD_ISSET(this->client[i], &fds) ? 1 : 0;
}

bool CServer::NonBlock(int fd, bool blocking)
{
		if (fd < 0)
		{
				return false;
		}
		int flags = fcntl(fd, F_GETFL, 0);
		if (flags < 0) 
		{
				return false;
		}
		flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
		return (fcntl(fd, F_SETFL, flags) == 0) ? true : false;
}


CServer::CServer(void)
{
		this->port = PORT;
		count = 0;
		for (int i = 0; i < NUMCLIENT; i++)
		{
				client[i] = -1;
		}

		this->sockfd = socket(AF_INET, SOCK_STREAM, 0);

		bzero((char*)&(this->addr), (int)sizeof(this->addr));
		this->addr.sin_family = AF_INET;
		this->addr.sin_port = htons(this->port);
		this->addr.sin_addr.s_addr = INADDR_ANY;

		if (bind(sockfd, (struct sockaddr*)&(this->addr), (int)sizeof(this->addr)) < 0)
		{
				perror("[error]");
		}
		this->NonBlock(this->sockfd, 0);
		listen(this->sockfd, NUMCLIENT);

		FD_ZERO(&s_client);

		FD_SET(this->sockfd, &s_client);
}

void CServer::AddClient(void)
{
		if (count == NUMCLIENT - 1)
		{
				printf("No PLace For Client");
		}


		struct sockaddr_in addr_client;
		socklen_t param;

		this->client[count] = accept(this->sockfd, (struct sockaddr *)&addr_client, &param);

		if (this->client[count] < 0)
		{
				printf("Accept error");
		}
		this->NonBlock(this->client[count], 0);
		FD_SET(this->client[this->count], &(this->s_client));
		count += 1;
#ifdef DEBUG
		std::cout << "Add client : " << this->client[this->count - 1] << std::endl;
#endif
}


void CServer::RemoveClient(int i)
{
#ifdef DEBUG
		std::cout << "Remove client : " << this->client[i] << std::endl;
#endif
		close(this->client[i]);
		FD_CLR(this->client[i], &(this->s_client));

		this->count--;
		for (int j = i; j < this->count; j++)
		{
				this->client[j] = this->client[j + 1];
				this->Table[j] = this->Table[j + 1];
		}
}


std::string CServer::ReadFrom(int i)
{
		if (this->client[i] >= 0)
		{
				char *buf;
				int r = 0;  // count of bytes which must read now
				int n = 0;  // count of bytes which must read after

				while(r < (int)sizeof(int))
				{
						r += recv(this->client[i], (&n) + r, (int)sizeof(int) - r, 0);
				}

#ifdef DEBUG
				std::cout << "bytes read: " << r << " and read: " << n << std::endl;
#endif
				r = 0;

				buf = (char *)malloc((n + 1) * (int)sizeof(char));
				if (buf == NULL)
				{
						std::cout << "Error in malloc" << std::endl;
				}

				while(r < n)
				{
						int p = recv(this->client[i], buf + r, n - r, 0);

						if (p < 0)
						{
							//std::cout << "error in recv" << std::endl;
							continue;
						}
						r += p; 
#ifdef DEBUG
						std::cout << r << std::endl;
#endif
				}
				buf[n] = '\0';
				return std::string(buf);
		}
		return std::string("");
}

void CServer::WriteTo(int i, std::string msg)
{
		if (this->client[i] >= 0)
		{
				while (IsReadyWrite(i) == 0){}
				int n = strlen(msg.c_str());
				send(this->client[i], &n, (int)sizeof(int), 0);
				send(this->client[i], msg.c_str(), n, 0);

		}
}

void CServer::Do(int i, std::string msg)
{
		Command com(msg);
		if (com.correct != 1)
		{	
				std::cout << "Incorrect command" << std::endl;
				this->WriteTo(i, "Incorrect command");
		}
		if (msg == "generate end")
		{
			CStudent * s = new CStudent[100];
			for (int i = 0; i < 100; i++)
			{
				s[i].name = std::to_string(i);
				s[i].rating = i%10;
				s[i].group = i;
				this->CommonTable.Add(&(s[i]));
			}
			this->WriteTo(i, "OK");
		}
		else if (msg == "load end")
		{
			this->CommonTable.FromFile("data");
			this->WriteTo(i, "OK");
		}
		else if (msg == "save end")
		{
			this->CommonTable.ToFile("data");
			this->WriteTo(i, "OK");
		}
		else if (com.command == "print")
		{
#ifdef DEBUG
#ifdef HTML
				std::cout << this->Table[i]->ToHTMLString(com.fields, com.sort);
#else
				std::cout << this->Table[i]->ToString(com.fields, com.sort);
#endif
#endif

#ifdef HTML
				this->WriteTo(i, this->Table[i]->ToHTMLString(com.fields, com.sort));
#else
				this->WriteTo(i, this->Table[i]->ToString(com.fields, com.sort));
#endif
		}
		else if (com.command == "select")
		{
				this->Table[i] = &CommonTable;
				for (int j = 0; j < (int)com.fields.size(); j++)
				{
						CTable *del = this->Table[i];
						if (com.fields[j] == "name")
						{
								this->Table[i] = this->Table[i]->Select(com.prefix);
								if (j > 0 && del != NULL)
								{
										delete del;
								}
						}
						else if (com.fields[j] == "rating")
						{
								for (int l = 0; l < (int)com.rating.size(); l++)
								{
										CTable *del1 = this->Table[i];
										CTable *p = del1->Select(com.rating[l].first, com.rating[l].second);
										this->Table[i] = this->Table[i]->Unique(p);
										if (l > 0 && del1 != NULL)
										{
												         delete del1;
										}
										if (p != NULL)
									    delete p;
								}
								if (j > 0 && del != NULL)
								{
										delete del;
								}
						}
						else if (com.fields[j] == "group")
						{
								this->Table[i] = this->Table[i]->Select(com.group);
								if (j > 0 && del != NULL)
								{
										delete del;
								}
						}
						std::cout << "TABLE PRINT DANGEROUS!!!" << std::endl;
						this->Table[i]->print("name");
						std::cout << "------------------------" << std::endl;
				}
				this->WriteTo(i, "OK");
		}
		else if (com.command == "reselect")
		{
            for (int j = 0; j < (int)com.fields.size(); j++)
            {
                    CTable *del = this->Table[i];
                    if (com.fields[j] == "name")
                    {
                            this->Table[i] = this->Table[i]->Select(com.prefix);
                            if (j > 0 && del != NULL)
                            {
                                    delete del;
                            }
                    }
                    else if (com.fields[j] == "rating")
                    {
                            for (int l = 0; l < (int)com.rating.size(); l++)
                            {
                                    CTable *del1 = this->Table[i];
                                    CTable *p = del1->Select(com.rating[l].first, com.rating[l].second);
                                    this->Table[i] = this->Table[i]->Unique(p);
                                    if (l > 0 && del1 != NULL)
                                    {
                                                     delete del1;
                                    }
                                    if (p != NULL)
                                    delete p;
                            }
                            if (j > 0 && del != NULL)
                            {
                                    delete del;
                            }
                    }
                    else if (com.fields[j] == "group")
                    {
                            this->Table[i] = this->Table[i]->Select(com.group);
                            if (j > 0 && del != NULL)
                            {
                                    delete del;
                            }
                    }
                    std::cout << "TABLE PRINT DANGEROUS!!!" << std::endl;
                    this->Table[i]->print("name");
                    std::cout << "------------------------" << std::endl;
            }
            this->WriteTo(i, "OK");
        }
		else if (com.command == "insert")
		{
				CStudent *s = new CStudent(com.prefix, com.rating[0].first, com.group[0]);
				this->CommonTable.Add(s);
				this->Table[i] = &(this->CommonTable);
				this->WriteTo(i, "OK");
		}
		else if (com.command == "remove")
		{
				CTable* rt = &CommonTable;
                for (int j = 0; j < (int)com.fields.size(); j++)
				{
						CTable *del = rt;
						if (com.fields[j] == "name")
						{
								rt = rt->Select(com.prefix);
								if (j > 0 && del != NULL)
								{
										delete del;
								}
						}
						else if (com.fields[j] == "rating")
						{
								for (int l = 0; l < (int)com.rating.size(); l++)
								{
										CTable *del1 = rt;
										CTable *p = del1->Select(com.rating[l].first, com.rating[l].second);
										rt = rt->Unique(p);
										if (l > 0 && del1 != NULL)
										{
												         delete del1;
										}
										if (p != NULL)
									    delete p;
								}
								if (j > 0 && del != NULL)
								{
										delete del;
								}
						}
						else if (com.fields[j] == "group")
						{
								rt = rt->Select(com.group);
								if (j > 0 && del != NULL)
								{
										delete del;
								}
						}
						std::cout << "TABLE PRINT DANGEROUS!!!" << std::endl;
						rt->print("name");
						std::cout << "------------------------" << std::endl;
				}
				this->WriteTo(i, "OK");

                for (int l = 0; l < HASHTABLESIZE; l++)
				{
						CEntry * entr = rt->hash[l];
						while(entr != NULL && entr->RateRoot != NULL)
						{
								CRateNode *r = entr->RateRoot;
								while(r != NULL && r->s != NULL)
								{
										this->CommonTable.Delete(r->s);
										r = r->next;
								}
								entr = entr->next;
						}
				}
				this->Table[i] = &(this->CommonTable);
				this->WriteTo(i, "OK");
		}
#ifdef DEBUG
		std::cout << msg << std::endl;
#endif
}


Command::Command(std::string str)
{
		this->correct = 1;
		int j = 0;
		int b = 0;
		std::vector<std::string> token;
		for (int i = 0; i < (int)str.length(); i++)
		{
				if (str[i] == ' ')
				{
						if (j < i)
						{
								token.push_back(str.substr(j, i - j));
						}
						j = i + 1;
				}
		}
		token.push_back(str.substr(j, str.length() - j));

		this->command = token[0];

		if (token[token.size() - 1] != std::string("end"))
		{
				this->correct = -1;
		}
		else
		{
				for (int i = 1; i < (int)token.size(); i++)
				{
						if (token[i] == "sort")
						{
								if (i == (int)token.size() - 3)
								{
										this->sort = token[i + 1];
								}
								else
								{
										this->correct = -1;
								}
								break;
						}
						else
						{
								std::size_t k = token[i].find("=");
								std::cout << "position of = in command: " << (int)k << std::endl;
								if (k == std::string::npos)
								{
										this->fields.push_back(token[i]);
								}
								else
								{
										std::string s1(token[i].substr(0, k));
#ifdef DEBUG
										std::cout << "Parsed field: " << s1 << std::endl;
#endif
										std::string s2(token[i].substr(k + 1, token[i].length() - k - 1));
#ifdef DEBUG
										std::cout << "Parsed field: " << s2 << std::endl;
#endif
										if (s1 == "name")
										{
												this->prefix = s2.substr(0, this->prefix.length() - 1);
												b =  1;
										}
										else if (s1 == "rating")
										{
												int j1 = 0;
												std::vector<std::string> ratings;
												for (int i = 0; i < (int)s2.length(); i++)
												{
														if (s2[i] == ',')
														{
																if (j1 < i)
																{
																		ratings.push_back(s2.substr(j1, i - j1));
																}
																j1 = i + 1;
														}
												}
												ratings.push_back(s2.substr(j1, s2.length() - j1));

												for (int i = 0; i < (int)ratings.size(); i++)
												{
														if ((k = ratings[i].find("-")) == std::string::npos)
														{
																int rate = Command::StrToInt(ratings[i]);
																this->rating.push_back(std::pair<double, double>(rate, rate));
														}
														else
														{
																std::string s1(ratings[i].substr(0, k));
																std::string s2(ratings[i].substr(k + 1, ratings[i].length() - k - 1));
																int rate1 = Command::StrToInt(s1);
																int rate2 = Command::StrToInt(s2);
																this->rating.push_back(std::pair<double, double>(rate1, rate2));
														}
												}
												b = 1;
										}
										else if (s1 == "group")
										{
												int j1 = 0;
												std::vector<std::string> groups;
												for (int i = 0; i < (int)s2.length(); i++)
												{
														if (s2[i] == ',')
														{
																if (j1 < i)
																{
																		groups.push_back(s2.substr(j1, i - j1));
																}
																j1 = i + 1;
														}
												}
												groups.push_back(s2.substr(j1, s2.length() - j1));

												for (int i = 0; i < (int)groups.size(); i++)
												{
														if ((k = groups[i].find("-")) == std::string::npos)
														{
																int group1 = Command::StrToInt(groups[i]);
																this->group.push_back(group1);
														}
														else
														{
																std::string s1(groups[i].substr(0, k));
																std::string s2(groups[i].substr(k + 1, groups[i].length() - k - 1));
																int group1 = Command::StrToInt(s1);
																int group2 = Command::StrToInt(s2);
																for(int groupin = group1; groupin <= group2; groupin++)
																{ 
																		this->group.push_back(groupin);
																}
														}
												}
												b = 1;
										}
										if (b == 0)
										{
											this->correct = -1;
										}
										this->fields.push_back(s1);
#ifdef DEBUG
										std::cout << "token[I] : " << s1 << std::endl;
#endif
								}
						}
				}
		}
}

int Command::StrToInt(std::string str)
{
		if (str == "*")
		{
				return INT32_MAX;
		}
		else
		{
				return std::stoi(str);
		}
}

void Command::print(void)
{
		std::cout << "corect : " << correct << std::endl;
		std::cout << command << " ";
		for (int i = 0; i < (int)this->fields.size(); i++)
		{
				std::cout << fields[i] << " ";
		}
		std::cout << std::endl;
		std::cout << "prefix: " << prefix << std::endl;
		for (int i = 0; i < (int)this->rating.size(); i++)
		{
				std::cout << rating[i].first << " and " << rating[i].second << " ";
		}
		std::cout << std::endl;
		for (int i = 0; i < (int)this->group.size(); i++)
		{
				std::cout << group[i] << " ";
		}
		std::cout << std::endl;

		std::cout << "sorted: " << sort << std::endl;
}
