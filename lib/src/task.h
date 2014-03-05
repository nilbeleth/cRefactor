#ifndef TASK_H
#define TASK_H



namespace refactor
{


/**
 *
 */
class Task
{
    public:
        /** Default destructor */
        virtual ~Task();

        static Task* construct();
    protected:
        /** Hide default constructor */
        Task() {}
    private:
};

}   // end namespace refactor
#endif // TASK_H
