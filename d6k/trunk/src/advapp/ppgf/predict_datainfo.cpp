#include "predict_datainfo.h"

#include <QFile>
#include <QDomNode>

CReadXml::CReadXml()
{
	map_PointInfo.clear();
	map_TagName.clear();
}

CReadXml::~CReadXml()
{

}


bool CReadXml::ReadGroupInfo(const QString& szPath)
{
	return true;
}

bool CReadXml::ReadPredictNodeInfo()
{
	return true;
}

bool CReadXml::ReadPredictFile()
{
	QXmlStreamReader xml;
	QFile file("powerpredict.xml");

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return false;
	}

	xml.setDevice(&file);

	QDomDocument document;

	if (!document.setContent(&file))
	{
		file.close();
		return false;
	}

	QDomElement root = document.documentElement();
	if (root.tagName() != "plant")
	{
		file.close();
		return false;
	}

	ReadStationData(xml);

	file.close();
	return true;
}

bool CReadXml::ReadStationData(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		reader.readNext();
		QString strTmp = reader.name().toString();

		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "p")
			{
				ReadPlantData(reader);
			}
			else if (strTmp == "w")
			{
				ReadWeatherData(reader);
			}
			else if (strTmp == "prdt")
			{
				ReadPredictData(reader);
			}
			else if (strTmp == "inverters")
			{
				ReadInverterData(reader);
			}
		}
		else if (reader.isEndElement() && strTmp == "plant")
		{
			break;
		}
	}
	return true;
}

bool CReadXml::ReadPlantData(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "p")
			{

				QString nName = reader.attributes().value("Name").toString();
				
				while (!reader.atEnd())
				{
					QString strTmp = reader.name().toString();
					if (reader.isStartElement())
					{
						QString strTmp = reader.name().toString();
						if (strTmp == "ADIN")
						{
							
							while (!reader.atEnd())
							{
								QString strTmp = reader.name().toString();
								if (reader.isStartElement())
								{
									QString strTmp = reader.name().toString();
									if (strTmp == "ai")
									{
										
										while (!reader.atEnd())
										{
											QString strTmp = reader.name().toString();
											if (reader.isStartElement())
											{
												QString strTmp = reader.name().toString();

												if (strTmp == "ai")
												{										
													int nID = reader.attributes().value("ID").toInt();
													plantPointinfo.nID = nID;
													QString szName = reader.attributes().value("Name").toString();
													plantPointinfo.szName = szName;
													QString szLinkedName = reader.attributes().value("LinkedName").toString();
													plantPointinfo.szTagName = szLinkedName;
													int nType = reader.attributes().value("Type").toInt();
													plantPointinfo.nType = nType;
													
												}
											}
											else if (reader.isEndElement() && strTmp == "ai")
											{
												break;
											}

											reader.readNext();
										}

									}
								}
								else if (reader.isEndElement() && strTmp == "ADIN")
								{
									break;
								}
								reader.readNext();
							}

						}
					}
					else if (reader.isEndElement() && strTmp == "data")
					{
						break;
					}
					reader.readNext();
				}


			}
		}
		else if (reader.isEndElement() && strTmp == "p")
		{
			break;
		}
		reader.readNext();
	}
	return true;
}

bool CReadXml::ReadWeatherData(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "w")
			{			
				QString nName = reader.attributes().value("Name").toString();
				
				while (!reader.atEnd())
				{
					QString strTmp = reader.name().toString();
					if (reader.isStartElement())
					{
						QString strTmp = reader.name().toString();
						if (strTmp == "ADIN")
						{
							
							while (!reader.atEnd())
							{
								QString strTmp = reader.name().toString();
								if (reader.isStartElement())
								{
									QString strTmp = reader.name().toString();
									if (strTmp == "ai")
									{
										
										while (!reader.atEnd())
										{
											QString strTmp = reader.name().toString();
											if (reader.isStartElement())
											{
												QString strTmp = reader.name().toString();

												if (strTmp == "ai")
												{
													
													int nID = reader.attributes().value("ID").toInt();
													weatherPointinfo.nID = nID;
													QString szName = reader.attributes().value("Name").toString();
													weatherPointinfo.szName = szName;
													QString szLinkedName = reader.attributes().value("LinkedName").toString();
													weatherPointinfo.szTagName = szLinkedName;
													int nType = reader.attributes().value("Type").toInt();
													weatherPointinfo.nType = nType;
													
													
												}
											}
											else if (reader.isEndElement())
											{
												break;
											}

											reader.readNext();
										}

									}
								}
								else if (reader.isEndElement() && strTmp == "ADIN")
								{
									break;
								}

								reader.readNext();
							}

						}
					}
					else if (reader.isEndElement())
					{
						break;
					}
					reader.readNext();
				}

			}
		}
		else if (reader.isEndElement() && strTmp == "w")
		{
			break;
		}
		reader.readNext();
	}
	return true;
}

bool CReadXml::ReadPredictData(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "prdt")
			{
				QString nName = reader.attributes().value("Name").toString();
				
				while (!reader.atEnd())
				{
					QString strTmp = reader.name().toString();
					if (reader.isStartElement())
					{
						QString strTmp = reader.name().toString();
						if (strTmp == "ADIN")
						{
							
							while (!reader.atEnd())
							{
								QString strTmp = reader.name().toString();
								if (reader.isStartElement())
								{
									QString strTmp = reader.name().toString();
									if (strTmp == "ao")
									{
										
										while (!reader.atEnd())
										{
											QString strTmp = reader.name().toString();
											if (reader.isStartElement())
											{
												QString strTmp = reader.name().toString();

												if (strTmp == "ao")
												{
													int nID = reader.attributes().value("ID").toInt();
													predictPointinfo.nID = nID;
													QString szName = reader.attributes().value("Name").toString();
													predictPointinfo.szName = szName;
													QString szLinkedName = reader.attributes().value("LinkedName").toString();
													predictPointinfo.szTagName = szLinkedName;
													int nType = reader.attributes().value("Type").toInt();
													predictPointinfo.nType = nType;
													
												}
											}
											else if (reader.isEndElement())
											{
												break;
											}

											reader.readNext();
										}

									}
								}
								else if (reader.isEndElement() && strTmp == "ADIN")
								{
									break;
								}

								reader.readNext();
							}

						}
					}
					else if (reader.isEndElement())
					{
						break;
					}
					reader.readNext();
				}

			}
		}
		else if (reader.isEndElement() && strTmp == "prdt")
		{
			break;
		}
		reader.readNext();
	}
	return true;
}

bool CReadXml::ReadInverterData(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "inverters")
			{
				int nSixe = reader.attributes().value("Count").toInt();
				QString szName = reader.attributes().value("Name").toString();
				
				while (!reader.atEnd())
				{
					QString strTmp = reader.name().toString();
					if (reader.isStartElement())
					{
						QString strTmp = reader.name().toString();
						if (strTmp == "inverter")
						{
							QString nName = reader.attributes().value("Name").toString();
							int nID = reader.attributes().value("ID").toInt();
							
							while (!reader.atEnd())
							{
								QString strTmp = reader.name().toString();
								if (reader.isStartElement())
								{
									QString strTmp = reader.name().toString();
									if (strTmp == "ADIN")
									{
										
										while (!reader.atEnd())
										{
											QString strTmp = reader.name().toString();
											if (reader.isStartElement())
											{
												QString strTmp = reader.name().toString();
												if (strTmp == "ai")
												{
													
													while (!reader.atEnd())
													{
														QString strTmp = reader.name().toString();
														if (reader.isStartElement())
														{
															QString strTmp = reader.name().toString();

															if (strTmp == "ai")
															{
																int nID = reader.attributes().value("ID").toInt();
																inverterAIPointinfo.nID = nID;
																QString szName = reader.attributes().value("Name").toString();
																inverterAIPointinfo.szName = szName;
																QString szLinkedName = reader.attributes().value("LinkedName").toString();
																inverterAIPointinfo.szTagName = szLinkedName;
																int nType = reader.attributes().value("Type").toInt();
																inverterAIPointinfo.nType = nType;
															}
														}
														else if (reader.isEndElement())
														{
															break;
														}

														reader.readNext();
													}


												}
												else if (strTmp == "di")
												{
													
													while (!reader.atEnd())
													{
														QString strTmp = reader.name().toString();
														if (reader.isStartElement())
														{
															QString strTmp = reader.name().toString();

															if (strTmp == "di")
															{
																int nID = reader.attributes().value("ID").toInt();
																inverterDIPointinfo.nID = nID;
																QString szName = reader.attributes().value("Name").toString();
																inverterDIPointinfo.szName = szName;
																QString szLinkedName = reader.attributes().value("LinkedName").toString();
																inverterDIPointinfo.szTagName = szLinkedName;
																int nType = reader.attributes().value("Type").toInt();
																inverterDIPointinfo.nType = nType;
															}
														}
														else if (reader.isEndElement())
														{
															break;
														}

														reader.readNext();
													}

												}
											}
											else if (reader.isEndElement() && strTmp == "ADIN")
											{
												break;
											}

											reader.readNext();
										}

									}
								}
								else if (reader.isEndElement())
								{
									break;
								}
								reader.readNext();
							}
						
						}
					}
					else if (reader.isEndElement() && strTmp == "inverters")
					{
						/*reader.readNext();
						QString strTmp1 = reader.name().toString();
						if (strTmp == "inverters")
						{
						break;
						}
						else
						{
						continue;
						}*/
						break;
					}
					reader.readNext();
				}
				
			}
		}
		else if (reader.isEndElement() && strTmp == "inverters")
		{
			break;
		}
		reader.readNext();
	}
	return true;
}

bool CReadXml::ReadData()
{
	return true;
}

bool CReadXml::ReadADIN()
{
	return true;
}

bool CReadXml::ReadAi()
{
	return true;
}

bool CReadXml::ReadDi()
{
	return true;
}


CReadDatabase::CReadDatabase()
{

}


CReadDatabase::~CReadDatabase()
{

}