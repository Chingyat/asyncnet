#if !defined(ASYNCNET_EXECUTION_CONTEXT_HPP)
#define ASYNCNET_EXECUTION_CONTEXT_HPP

#include <memory>
#include <vector>
#include <mutex>
#include <exception>

namespace asyncnet {

    class execution_context;

    class service_already_exist : public std::logic_error {
    public:
        service_already_exist()
            : std::logic_error("service_already_exist")
        {}
    };

    template <typename Service>
    Service& use_service(execution_context &context);

    template <typename Service, typename ...Args>
    Service& make_service(execution_context &context, Args&& ...args);

    template <typename Service>
    bool has_service(const execution_context &context);


    class execution_context {
        template <typename Service>
        friend Service& use_service(execution_context &context);

        template <typename Service, typename ...Args>
        friend  Service& make_service(execution_context &context, Args&& ...args);

        template <typename Service>
        friend  bool has_service(const execution_context &context);
    public:
        class service;

        execution_context() = default;
        execution_context(const execution_context &) = delete;
        execution_context &operator=(const execution_context) = delete;


        enum fork_event
        {

        };

        void notify_fork(fork_event e);

    protected:
        ~execution_context() noexcept;

        void shutdown() noexcept;
        void destroy() noexcept;

    private:
        using service_index_type = unsigned long;

        mutable std::recursive_mutex _mutex;
        std::vector<std::pair<std::unique_ptr<service>, service_index_type> > _services;
    };



}

#endif
