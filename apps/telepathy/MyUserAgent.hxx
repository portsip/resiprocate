/*
 * Copyright (C) 2015 Daniel Pocock http://danielpocock.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MyUserAgent_hxx
#define MyUserAgent_hxx

#if defined(HAVE_CONFIG_H)
  #include "config.h"
#endif

#include <resip/recon/UserAgent.hxx>
#include <resip/stack/Pidf.hxx>

#include "Connection.hxx"
#include "MyInstantMessage.hxx"

namespace tr {

class Connection;
class MyInstantMessage;

class MyUserAgent : public QObject, public recon::UserAgent, public resip::ThreadIf
{
   Q_OBJECT
public:
   MyUserAgent(recon::ConversationManager* conversationManager, std::shared_ptr<recon::UserAgentMasterProfile> profile, Connection& connection, std::shared_ptr<MyInstantMessage> instantMessage);
   void onApplicationTimer(unsigned int id, std::chrono::duration<double> duration, unsigned int seq) override;
   void onSubscriptionTerminated(recon::SubscriptionHandle handle, unsigned int statusCode) override;
   void onSubscriptionNotify(recon::SubscriptionHandle handle, const resip::Data& notifyData) override;
   void onSuccess(resip::ClientRegistrationHandle h, const resip::SipMessage& response) override;
   void onRemoved(resip::ClientRegistrationHandle, const resip::SipMessage& response) override;
   void onFailure(resip::ClientRegistrationHandle, const resip::SipMessage& response) override;
   int onRequestRetry(resip::ClientRegistrationHandle, int retrySeconds, const resip::SipMessage& response) override;
   void thread() override;
   virtual void setStatus(uint newStatus, uint reason);
   virtual void stop();
   
private:
   std::vector<resip::Pidf::Tuple> getTuplesFromXML(const resip::Data& notifyData);
   
signals:
   void setContactStatus(const QString& identifier, const QString& status);

private:
   tr::Connection& mConnection;
   volatile bool mStopping;
};

}


#endif

