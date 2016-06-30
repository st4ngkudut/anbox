/*
 * Copyright (C) 2016 Simon Fels <morphis@gravedo.de>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef ANBOX_BRIDGE_MESSAGE_PROCESSOR_H_
#define ANBOX_BRIDGE_MESSAGE_PROCESSOR_H_

#include "anbox/network/message_processor.h"
#include "anbox/network/message_sender.h"
#include "anbox/bridge/pending_call_cache.h"

#include <memory>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/message_lite.h>

namespace anbox {
namespace protobuf {
namespace bridge {
class Invocation;
} // namespace bridge
} // namespace protobuf
namespace bridge {
class Invocation
{
public:
    Invocation(anbox::protobuf::bridge::Invocation const& invocation) :
        invocation_(invocation) {}

    const ::std::string& method_name() const;
    const ::std::string& parameters() const;
    google::protobuf::uint32 id() const;
private:
    anbox::protobuf::bridge::Invocation const& invocation_;
};

class MessageProcessor : public network::MessageProcessor {
public:
    MessageProcessor(const std::shared_ptr<network::MessageSender> &sender,
                     const std::shared_ptr<PendingCallCache> &pending_calls);
    ~MessageProcessor();

    bool process_data(const std::vector<std::uint8_t> &data) override;

    void send_response(::google::protobuf::uint32 id, google::protobuf::MessageLite *response);

    virtual void dispatch(Invocation const& invocation) = 0;
    virtual void process_event_sequence(const std::string &event) = 0;

private:
    std::shared_ptr<network::MessageSender> sender_;
    std::vector<std::uint8_t> buffer_;
    std::shared_ptr<PendingCallCache> pending_calls_;
};
} // namespace bridge
} // namespace anbox

#endif